#pragma once

#include <ghudcpp/draw/draw_list.h>
namespace GHUD {
	DrawList::DrawList() : m_ElementList(ElementVector<Element::Base*>()) {}

	DrawList::~DrawList() {
		m_ElementList.Clear();
	}

	void DrawList::FrameStart() {
		assert(m_ElementList.Size() == 0 && "Draw List must be cleared before frame start!");
		if (m_PreviousSize > 0) {
			m_ElementList.Allocate(m_PreviousSize); // allocate depending on previous array size for performance
		}
	}

	void DrawList::FrameEnd() {
		for (auto obj : m_HeapElements) {
			delete obj.m_Element;
		}
		this->Clear();
	}

	void DrawList::Clear() {
		m_PreviousSize = m_ElementList.Size();
		m_ElementList.Clear();
		m_HeapElements.Clear();
		m_Lines.Clear();
		m_Rects.Clear();
		m_Images.Clear();
		m_Texts.Clear();
	}

	Element::Line& DrawList::DrawLine(const Element::Line& line) {
		m_Lines.PushBack(line);
		m_ElementList.PushBack(&m_Lines.Last());
		return m_Lines.Last();
	}
	Element::Line& DrawList::DrawLine(fvec2 m_PointA, fvec2 m_PointB, RGBAColor m_Color) {
		Element::Line obj{};
		obj.m_PointA = m_PointA;
		obj.m_PointB = m_PointB;
		obj.m_Color = m_Color;
		m_Lines.PushBack(std::move(obj));
		m_ElementList.PushBack(&obj);
		return m_Lines.Last();
	}

	Element::Rect& DrawList::DrawRect(const Element::Rect& rect) {
		m_Rects.PushBack(rect);
		m_ElementList.PushBack(&m_Rects.Last());
		return m_Rects.Last();
	}
	Element::Rect& DrawList::DrawRect(const Element::Transform& m_Transform, const TextureObject& m_Texture, RGBAColor m_Color) {
		Element::Rect obj{};
		obj.m_Transform = m_Transform;
		obj.m_Color = m_Color;
		m_Rects.PushBack(std::move(obj));
		m_ElementList.PushBack(&m_Rects.Last());
		return  m_Rects.Last();
	}
}