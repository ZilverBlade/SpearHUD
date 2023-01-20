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

	const Element::Rect DrawList::DrawRect(const Element::Transform& m_Transform, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset) {
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

	const Element::Image DrawList::DrawImage(const Element::Transform& m_Transform, const TextureObject& m_Texture, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset, fvec2 m_GlobalUVOffsetMin, fvec2 m_GlobalUVOffsetMax) {
		Element::Image obj{};
		obj.m_Transform = m_Transform;
		obj.m_Color = m_Color;
		obj.m_Layer = m_Layer;
		obj.m_GlobalUVOffsetMin = m_GlobalUVOffsetMin;
		obj.m_GlobalUVOffsetMax = m_GlobalUVOffsetMax;
		obj.m_AnchorOffset = m_AnchorOffset;
		return DrawImage(obj);
	}

}