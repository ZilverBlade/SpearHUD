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
<<<<<<< HEAD
		assert(m_DrawList.size() == 0 && "Draw List must be cleared before frame start!");
		m_DrawList.get_allocator().allocate(m_PreviousSize);
=======
		assert(mDrawList.size() == 0 && "Draw List must be cleared before frame start!");
>>>>>>> 2c9f03cdecc6f6ddbddb97a93cad5aa7df7af018
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

<<<<<<< HEAD
	const Element::Rect DrawList::DrawRect(const Transform& m_Transform, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset) {
=======
	const Element::Rect DrawList::DrawRect(const Transform& mTransform, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset) {
>>>>>>> 2c9f03cdecc6f6ddbddb97a93cad5aa7df7af018
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

<<<<<<< HEAD
	const Element::Image DrawList::DrawImage(const Transform& m_Transform, const TextureObject& m_Texture, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset, fvec2 m_UVMin, fvec2 m_UVMax) {
		Element::Image obj{};
		obj.m_Transform = m_Transform;
		obj.m_Transform.m_Position += m_TransformOffset.m_Position;
		obj.m_Transform.m_Rotation += m_TransformOffset.m_Rotation;
		obj.m_Transform.m_Scale += m_TransformOffset.m_Scale;
		obj.m_Transform.m_TransformOffset += m_TransformOffset.m_TransformOffset;
		obj.m_Color = m_Color;
		obj.m_Layer = m_Layer + m_LayerOffset;
		obj.m_UVMin = m_UVMin;
		obj.m_UVMax = m_UVMax;
		obj.m_AnchorOffset = m_AnchorOffset + m_AnchorOffsetOffset;
		return DrawImage(obj);
	}

	const Element::Panel DrawList::BeginPanel(const Transform& m_Transform, LayerIndex m_Layer, fvec2 m_AnchorOffset) {
		m_TransformOffset.m_Position += m_Transform.m_Position;
		m_TransformOffset.m_Rotation += m_Transform.m_Rotation;
		m_TransformOffset.m_Scale += m_Transform.m_Scale;
		m_TransformOffset.m_TransformOffset += m_Transform.m_TransformOffset;

		m_LayerOffset += m_Layer;
		m_AnchorOffsetOffset += m_AnchorOffset;
		
		Element::Panel obj{};
		obj.m_AnchorOffset = m_AnchorOffset;
		obj.m_Layer = m_Layer;
		obj.m_Transform = m_Transform;
		return obj;
	}

	const void DrawList::EndPanel(const Element::Panel& panel) {
		m_TransformOffset.m_Position -= panel.m_Transform.m_Position;
		m_TransformOffset.m_Rotation -= panel.m_Transform.m_Rotation;
		m_TransformOffset.m_Scale -= panel.m_Transform.m_Scale;
		m_TransformOffset.m_TransformOffset -= panel.m_Transform.m_TransformOffset;

		m_LayerOffset -= panel.m_Layer;
		m_AnchorOffsetOffset -= panel.m_AnchorOffset;
=======
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

>>>>>>> 2c9f03cdecc6f6ddbddb97a93cad5aa7df7af018
	}

}