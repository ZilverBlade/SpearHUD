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
		pushTransform.mPosition = mPanel.mTransform.mPosition;
		pushTransform.mAnchorOffset = mPanel.mTransform.mTransformOffset;
		pushTransform.mLayerOffset = mPanel.mLayer;

		// anchor limits are [-1, 1] instead of [0, 1]
		fvec2 anchorCoordsOffset = Utils::ConvertScreenCoordToGPUCoord(mPanel.mTransform.mPosition);
		fvec2 anchorLim = mPanel.mTransform.mScale;
		pushTransform.mAnchorAreaLimMin = -anchorLim + anchorCoordsOffset;
		pushTransform.mAnchorAreaLimMax = anchorLim + anchorCoordsOffset;

		StackPushTransform applyTransform = pushTransform;
		applyTransform.mPosition = applyTransform.mPosition + mStackTransform.GetApplyData().mPosition;
		applyTransform.mTransform = applyTransform.mTransform * mStackTransform.GetApplyData().mTransform;

		applyTransform.mAnchorOffset = applyTransform.mAnchorOffset * Math::Abs(mStackTransform.GetApplyData().mAnchorOffset); // abs to avoid making negative values positive by accident
		applyTransform.mLayerOffset += mStackTransform.GetApplyData().mLayerOffset;

		applyTransform.mAnchorAreaLimMin = applyTransform.mAnchorAreaLimMin * mStackTransform.GetApplyData().mAnchorAreaLimMin;
		applyTransform.mAnchorAreaLimMax = applyTransform.mAnchorAreaLimMax * mStackTransform.GetApplyData().mAnchorAreaLimMax;
		mStackTransform.Push(pushTransform, applyTransform);
	}

	void DrawList::EndPanel() {
		StackPushTransform unApplyTransform = mStackTransform.GetApplyData();
		unApplyTransform.mPosition = unApplyTransform.mPosition - mStackTransform.GetBackData().mPosition;
		unApplyTransform.mTransform = unApplyTransform.mTransform * Math::Inverse(mStackTransform.GetBackData().mTransform);
		unApplyTransform.mAnchorOffset = unApplyTransform.mAnchorOffset / Math::Abs(mStackTransform.GetBackData().mAnchorOffset);
		unApplyTransform.mLayerOffset -= mStackTransform.GetBackData().mLayerOffset;
		
		unApplyTransform.mAnchorAreaLimMin = unApplyTransform.mAnchorAreaLimMin / mStackTransform.GetBackData().mAnchorAreaLimMin;
		unApplyTransform.mAnchorAreaLimMax = unApplyTransform.mAnchorAreaLimMax / mStackTransform.GetBackData().mAnchorAreaLimMax;
		mStackTransform.Pop(unApplyTransform);
	}

	void DrawList::Draw(DrawInfo drawInfo) {
		// apply stack pushes
		drawInfo.mData.mRotationMatrix = drawInfo.mData.mRotationMatrix * mStackTransform.GetApplyData().mTransform;
		drawInfo.mData.mPosition = drawInfo.mData.mPosition + mStackTransform.GetApplyData().mPosition;
		drawInfo.mData.mAnchorOffset = drawInfo.mData.mAnchorOffset * mStackTransform.GetApplyData().mAnchorOffset;
		drawInfo.mLayer += mStackTransform.GetApplyData().mLayerOffset;
		mDrawList.emplace(drawInfo);
	}

}