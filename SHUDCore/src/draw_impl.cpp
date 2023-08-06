#pragma once

#include <shudcpp/draw/draw_list.h>
#include <shudcpp/utils/helper.h>
#include <shudcpp/context.h>

namespace SHUD {
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
		Draw(DrawInfo{ line.mLayer, 0, 0, line.GenerateDrawData(&ctx->GetContextData()), 1 });
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
		DrawData data = rect.GenerateDrawData(&ctx->GetContextData());
		Draw(DrawInfo{ rect.mLayer, 0, 0, std::move(data), 1 });
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
		DrawData data = img.GenerateDrawData(&ctx->GetContextData());
		Draw(DrawInfo{ img.mLayer, img.mTexture.mAtlas.GetTextureID(), 0, std::move(data), 1 });
		return img;
	}

	const Element::Image DrawList::DrawImage(const Transform& mTransform, const TextureObject& mTexture, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset, fvec2 mGlobalUVOffsetMin, fvec2 mGlobalUVOffsetMax) {
		Element::Image obj{};
		obj.mTransform = mTransform;
		obj.mTexture = mTexture;
		obj.mColor = mColor;
		obj.mLayer = mLayer;
		obj.mGlobalUVOffsetMin = mGlobalUVOffsetMin;
		obj.mGlobalUVOffsetMax = mGlobalUVOffsetMax;
		obj.mAnchorOffset = mAnchorOffset;
		return DrawImage(obj);
	}

	SHUD_API const Element::Text DrawList::DrawText(const Element::Text& txt) {
		static std::hash<std::string> hasher = std::hash<std::string>();
		size_t hash = hasher(txt.mText) + 0xAE * static_cast<uint32_t>(txt.mFormatting.mVAlignment) - 0x6B * static_cast<uint32_t>(txt.mFormatting.mHAlignment);
		void* bufferID = (void*)ctx->AllocateTextBuffer(hash, txt.mText, txt.mFormatting);
		DrawData data = txt.GenerateDrawData(&ctx->GetContextData());
		Draw(DrawInfo{ txt.mLayer, ctx->GetFontAtlasTextureID(), (BufferID)bufferID, std::move(data), static_cast<uint32_t>(txt.mText.size()) });
		return txt;
	}

	SHUD_API const Element::Text DrawList::DrawText(const Transform& mTransform, RGBAColor mColor, LayerIndex mLayer, const TextFormatting& mFormatting, const FormattedText& mText, fvec2 mAnchorOffset) {
		Element::Text  obj{};
		obj.mTransform = mTransform;
		obj.mColor = mColor;
		obj.mLayer = mLayer;
		obj.mAnchorOffset = mAnchorOffset;
		obj.mText = mText;
		obj.mFormatting = mFormatting;
		return DrawText(obj);
	}

	const Element::Button DrawList::DrawButton(const Element::Button& btn) {
		DrawData data = btn.GenerateDrawData(&ctx->GetContextData());
		data.mID = btn.mElementID;
		Element::Button btncpy = btn;
		if (ctx->GetContextData().selectedObject == btn.mElementID) {
			if (ctx->GetIO().mButtonState[(int)MouseButtonCode::Button_Left] == true) {
				data.mSubUVOffsetA = btncpy.mTexture.mPressTextureCoords.mUVOffsetMin;
				data.mSubUVOffsetB = btncpy.mTexture.mPressTextureCoords.mUVOffsetMax;
				btncpy.mPressState |= SHUD_PRESS_STATE_FLAG_PRESSED;
				if (ctx->GetIOLastFrame().mButtonState.at((int)MouseButtonCode::Button_Left) == false) {
					btncpy.mPressState |= SHUD_PRESS_STATE_FLAG_CLICKED;
				}
			} else {
				data.mSubUVOffsetA = btncpy.mTexture.mHoverTextureCoords.mUVOffsetMin;
				data.mSubUVOffsetB = btncpy.mTexture.mHoverTextureCoords.mUVOffsetMax;
				btncpy.mPressState |= SHUD_PRESS_STATE_FLAG_HOVERING;
			}
		}
		Draw(DrawInfo{ btn.mLayer, btn.mTexture.mAtlas.GetTextureID(), 0, std::move(data), 1 });
		return btncpy;
	}

	void DrawList::BeginPanel(const Element::Panel& mPanel) {
		StackPushTransform pushTransform{};
		fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mPanel.mTransform.mScale, ctx->GetContextData().mInvResolution);
		//uvScale.x *= ctx->GetContextData().mAspectRatio;
		//uvScale.y *= ctx->GetContextData().mInvAspectRatio;
		pushTransform.mTransform = Math::Transform2x2(uvScale, 0.0);
		pushTransform.mAbsPosition = Utils::ConvertScreenCoordToGPUCoord(mPanel.mTransform.mPosition, ctx->GetContextData().mInvResolution);
		//pushTransform.mAbsPosition.x *= ctx->GetContextData().mAspectRatio;
		//pushTransform.mAbsPosition.y *= ctx->GetContextData().mInvAspectRatio;
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


		if (mStackTransform.HasStack()) {
			// not sure if scaling the elements inside the panel makes sense, since issues with pixel size arise, 
			// where the true full screen resolution maps to the content area, resulting in very counter-intuitive transforms
			drawInfo.mData.mRotationMatrix = applyTransform.mTransform * drawInfo.mData.mRotationMatrix;

			// override anchor if a transform is applying one to prioritize the positioning
			drawInfo.mData.mPosition = applyTransform.mTransform * drawInfo.mData.mPosition + drawInfo.mData.mAnchorOffset;
			drawInfo.mData.mAnchorOffset = applyTransform.mAnchorOffset;
		}
		//drawInfo.mData.mPosition = drawInfo.mData.mPosition * applyTransform.mAnchorAreaScale; // normalize the positioning
		drawInfo.mLayer += applyTransform.mLayerOffset;
		mDrawList.emplace(drawInfo);
	}

}