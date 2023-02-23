#pragma once

#include <ghudcpp/draw/draw_list.h>
#include <ghudcpp/utils/helper.h>
#include <ghudcpp/context.h>

namespace GHUD {
	DrawList::DrawList(Context* ctx_) : ctx(ctx_) {
	
	}

	DrawList::~DrawList() {
		this->Clear();
	}

	void DrawList::FrameStart() {
		assert(mDrawList.size() == 0 && "Draw List must be cleared before frame start!");
		mDrawList.get_allocator().allocate(mPreviousSize);
	}

	void DrawList::FrameEnd() {

	}

	void DrawList::Clear() {
		mPreviousSize = mDrawList.size();
		mDrawList.clear();
		mStackTransform = {};
	}

	const Element::Line DrawList::DrawLine(const Element::Line& line) {
		Draw(DrawInfo{ line.mLayer, 0, line.GenerateDrawData(&ctx->GetGlobalContextInfo()) });
		return line;
	}
	const Element::Line DrawList::DrawLine(fvec2 mPointA, fvec2 mPointB, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset) {
		Element::Line obj{};
		obj.mPointA = mPointA;
		obj.mPointB = mPointB;
		obj.mColor = mColor;
		obj.mLayer = mLayer;
		obj.mAnchorOffset = mAnchorOffset;
		return DrawLine(obj);
	}

	const Element::Rect DrawList::DrawRect(const Element::Rect& rect) {
		DrawData data = rect.GenerateDrawData(&ctx->GetGlobalContextInfo());
		data.mID = mDrawList.size();
		Draw(DrawInfo{ rect.mLayer, 0, std::move(data) });
		return rect;
	}


	const Element::Rect DrawList::DrawRect(const Transform& mTransform, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset) {
		Element::Rect obj{};
		obj.mTransform = mTransform;
		obj.mColor = mColor;
		obj.mLayer = mLayer;
		obj.mAnchorOffset = mAnchorOffset;
		return DrawRect(obj);
	}
	const Element::Image DrawList::DrawImage(const Element::Image& img) {
		DrawData data = img.GenerateDrawData(&ctx->GetGlobalContextInfo());
		data.mID = mDrawList.size();
		Draw(DrawInfo{ img.mLayer, img.mTexture.mAtlas.GetTextureID(), std::move(data) });
		return img;
	}

	const Element::Image DrawList::DrawImage(const Transform& mTransform, const TextureObject& mTexture, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset, fvec2 mGlobalUVOffsetMin, fvec2 mGlobalUVOffsetMax) {
		Element::Image obj{};
		obj.mTransform = mTransform;
		obj.mColor = mColor;
		obj.mLayer = mLayer;
		obj.mGlobalUVOffsetMin = mGlobalUVOffsetMin;
		obj.mGlobalUVOffsetMax = mGlobalUVOffsetMax;
		obj.mAnchorOffset = mAnchorOffset;
		return DrawImage(obj);
	}

	void DrawList::BeginPanel(const Element::Panel& mPanel) {
		StackPushTransform pushTransform{};
		pushTransform.mTransform = Math::Transform2x2(mPanel.mTransform.mScale, 0.0);
		pushTransform.mAbsPosition = Utils::ConvertScreenCoordToGPUCoord(mPanel.mTransform.mPosition);
		pushTransform.mLayerOffset = mPanel.mLayer;

		pushTransform.mAnchorAreaScale = mPanel.mTransform.mScale;

		StackPushTransform newApplyTransform = pushTransform;
		StackPushTransform oldApplyTransform = mStackTransform.GetApplyData();
		newApplyTransform.mAbsPosition += oldApplyTransform.mAbsPosition;
		newApplyTransform.mTransform = newApplyTransform.mTransform * oldApplyTransform.mTransform;

		// anchor limits are [-1, 1] unlike positions which are [0, 1] 
		newApplyTransform.mAnchorOffset = newApplyTransform.mAnchorOffset + newApplyTransform.mAbsPosition;
		newApplyTransform.mLayerOffset += oldApplyTransform.mLayerOffset;

		newApplyTransform.mAnchorAreaScale = newApplyTransform.mAnchorAreaScale * oldApplyTransform.mAnchorAreaScale;
		mStackTransform.Push(pushTransform, newApplyTransform);
	}

	void DrawList::EndPanel() {
		StackPushTransform unApplyTransform = mStackTransform.GetApplyData();
		StackPushTransform backTransform = mStackTransform.GetBackData();
		unApplyTransform.mAbsPosition = unApplyTransform.mAbsPosition - backTransform.mAbsPosition;
		unApplyTransform.mTransform = unApplyTransform.mTransform * Math::Inverse(backTransform.mTransform);
		unApplyTransform.mAnchorOffset = unApplyTransform.mAnchorOffset - backTransform.mAnchorOffset;
		unApplyTransform.mLayerOffset -= backTransform.mLayerOffset;
		
		unApplyTransform.mAnchorAreaScale = unApplyTransform.mAnchorAreaScale / backTransform.mAnchorAreaScale;
		mStackTransform.Pop(unApplyTransform);
	}

	void DrawList::Draw(DrawInfo drawInfo) {
		// apply stack pushes
		StackPushTransform applyTransform = mStackTransform.GetApplyData();
		drawInfo.mData.mRotationMatrix = drawInfo.mData.mRotationMatrix * applyTransform.mTransform;
		if (applyTransform.mAnchorOffset.x != 0.0 || applyTransform.mAnchorOffset.y != 0.0) {
			// override anchor if a transform is applying one to prioritize the positioning
			drawInfo.mData.mPosition = drawInfo.mData.mPosition + drawInfo.mData.mAnchorOffset;
			drawInfo.mData.mAnchorOffset = applyTransform.mAnchorOffset;
		}
		drawInfo.mData.mPosition = drawInfo.mData.mPosition * applyTransform.mAnchorAreaScale; // normalize the positioning
		drawInfo.mLayer += applyTransform.mLayerOffset;
		mDrawList.emplace(drawInfo);
	}

}