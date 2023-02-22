#pragma once

#include <ghudcpp/draw/draw_list.h>
#include <ghudcpp/context.h>

namespace GHUD {
	DrawList::DrawList(Context* ctx_) : ctx(ctx_) {
	
	}

	DrawList::~DrawList() {
		this->Clear();
	}

	void DrawList::FrameStart() {
		assert(mDrawList.size() == 0 && "Draw List must be cleared before frame start!");
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

	void DrawList::BeginPanel(const Transform& mTransform) {
		StackPushTransform pushTransform = { Math::Transform2x2(mTransform.mScale, 0.0), mTransform.mPosition };
		StackPushTransform applyTransform = pushTransform;
		applyTransform.mPosition = applyTransform.mPosition + mStackTransform.GetApplyData().mPosition;
		applyTransform.mTransform = applyTransform.mTransform * mStackTransform.GetApplyData().mTransform;
		mStackTransform.Push(pushTransform, applyTransform);
	}

	void DrawList::EndPanel() {
		StackPushTransform unApplyTransform = mStackTransform.GetApplyData();
		unApplyTransform.mPosition = unApplyTransform.mPosition - mStackTransform.GetBackData().mPosition;
		unApplyTransform.mTransform = unApplyTransform.mTransform * Math::Inverse(mStackTransform.GetBackData().mTransform);
		mStackTransform.Pop(unApplyTransform);
	}

	void DrawList::Draw(DrawInfo drawInfo) {
		// apply stack pushes
		drawInfo.mData.mRotationMatrix = drawInfo.mData.mRotationMatrix * mStackTransform.GetApplyData().mTransform;
		drawInfo.mData.mPosition = drawInfo.mData.mPosition + mStackTransform.GetApplyData().mPosition;

		mDrawList.emplace(drawInfo);

	}

}