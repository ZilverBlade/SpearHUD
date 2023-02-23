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
		assert(m_DrawList.size() == 0 && "Draw List must be cleared before frame start!");
		m_DrawList.get_allocator().allocate(m_PreviousSize);
	}

	void DrawList::FrameEnd() {
	}

	void DrawList::Clear() {
		m_PreviousSize = m_DrawList.size();
		m_DrawList.clear();
	}

#define DrawDataLast m_DrawData[m_DrawData.size() - 1]

	const Element::Line DrawList::DrawLine(const Element::Line& line) {
		m_DrawList.emplace(DrawInfo{ line.m_Layer, 0, line.GenerateDrawData(&ctx->GetGlobalContextInfo()) });
		return line;
	}
	const Element::Line DrawList::DrawLine(fvec2 m_PointA, fvec2 m_PointB, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset) {
		Element::Line obj{};
		obj.m_PointA = m_PointA;
		obj.m_PointB = m_PointB;
		obj.m_Color = m_Color;
		obj.m_Layer = m_Layer;
		obj.m_AnchorOffset = m_AnchorOffset;
		return DrawLine(obj);
	}

	const Element::Rect DrawList::DrawRect(const Element::Rect& rect) {
		DrawData data = rect.GenerateDrawData(&ctx->GetGlobalContextInfo());
		data.m_ID = m_DrawList.size();
		m_DrawList.emplace(DrawInfo{ rect.m_Layer, 0, std::move(data) });
		return rect;
	}

	const Element::Rect DrawList::DrawRect(const Transform& m_Transform, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset) {
		Element::Rect obj{};
		obj.m_Transform = m_Transform;
		obj.m_Color = m_Color;
		obj.m_Layer = m_Layer;
		obj.m_AnchorOffset = m_AnchorOffset;
		return DrawRect(obj);
	}
	const Element::Image DrawList::DrawImage(const Element::Image& img) {
		DrawData data = img.GenerateDrawData(&ctx->GetGlobalContextInfo());
		data.m_ID = m_DrawList.size();
		m_DrawList.emplace(DrawInfo{ img.m_Layer, img.m_Texture.m_Atlas.GetTextureID(), std::move(data) });
		return img;
	}

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
	}

}