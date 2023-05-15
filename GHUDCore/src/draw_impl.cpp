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

	const Element::Button DrawList::DrawButton(const Element::Button& btn) {
		DrawData data = btn.GenerateDrawData(&ctx->GetGlobalContextInfo());
		data.mID = btn.mElementID;
		Element::Button btncpy = btn;
		if (ctx->GetGlobalContextInfo().selectedObject == btn.mElementID) {
			if (ctx->GetIO().mButtonState[(int)MouseButtonCode::Button_Left] == true) {
				data.mSubUVOffsetA = btncpy.mTexture.mPressTextureCoords.mUVOffsetMin;
				data.mSubUVOffsetB = btncpy.mTexture.mPressTextureCoords.mUVOffsetMax;
				btncpy.mPressState |= GHUD_PRESS_STATE_FLAG_PRESSED;
				if (ctx->GetIOLastFrame().mButtonState.at((int)MouseButtonCode::Button_Left) == false) {
					btncpy.mPressState |= GHUD_PRESS_STATE_FLAG_CLICKED;
				}
			} else {
				data.mSubUVOffsetA = btncpy.mTexture.mHoverTextureCoords.mUVOffsetMin;
				data.mSubUVOffsetB = btncpy.mTexture.mHoverTextureCoords.mUVOffsetMax;
				btncpy.mPressState |= GHUD_PRESS_STATE_FLAG_HOVERING;
			}
		}
		Draw(DrawInfo{ btn.mLayer, btn.mTexture.mAtlas.GetTextureID(), std::move(data) });
		return btncpy;
	}

	void DrawList::BeginPanel(const Element::Panel& mPanel) {
		StackPushTransform pushTransform{};
		fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mPanel.mTransform.mScale, ctx->GetGlobalContextInfo().mInvResolution);
		pushTransform.mTransform = Math::Transform2x2(uvScale, 0.0);
		pushTransform.mAbsPosition = Utils::ConvertScreenCoordToGPUCoord(mPanel.mTransform.mPosition, ctx->GetGlobalContextInfo().mInvResolution);
		pushTransform.mLayerOffset = mPanel.mLayer;

		pushTransform.mAnchorAreaScale = uvScale;

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

		// not sure if scaling the elements inside the panel makes sense, since issues with pixel size arise, 
		// where the true full screen resolution maps to the content area, resulting in very counter-intuitive transforms
		
		//drawInfo.mData.mRotationMatrix = drawInfo.mData.mRotationMatrix * applyTransform.mTransform;

		if (mStackTransform.HasStack()) {
			// override anchor if a transform is applying one to prioritize the positioning
			drawInfo.mData.mPosition = drawInfo.mData.mPosition + drawInfo.mData.mAnchorOffset;
			drawInfo.mData.mAnchorOffset = applyTransform.mAnchorOffset;
		}
		//drawInfo.mData.mPosition = drawInfo.mData.mPosition * applyTransform.mAnchorAreaScale; // normalize the positioning
		drawInfo.mLayer += applyTransform.mLayerOffset;
		mDrawList.emplace(drawInfo);
	}

}