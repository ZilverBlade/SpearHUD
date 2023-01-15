#pragma once

#include <ghudcpp/draw/draw_list.h>
namespace GHUD {
	DrawList::DrawList() : m_ElementList(DrawAllocator()) {}

	DrawList::~DrawList() {
		m_ElementList.Clear();
	}

	void DrawList::FrameStart() {
		assert(m_ElementList.Size() == 0 && "Draw List must be cleared before frame start!");
		if (this->m_PreviousSize > 0) {
			this->m_ElementList.Allocate(m_PreviousSize); // allocate depending on previous array size for performance
		}
	}

	void DrawList::FrameEnd() {
		this->Clear();
	}

	Element::Rect& DrawList::DrawRect(const Element::Rect& rect) {
		Element::Rect* rectPTR = new Element::Rect(rect);
		m_ElementList.PushBack(rectPTR);
	}
	Element::Rect& DrawList::DrawRect(const Element::Transform& m_Transform, TextureObject m_Atlas, RGBAColor m_Color) {
		Element::Rect* rectPTR = new Element::Rect();
		rectPTR->m_Transform = m_Transform;
		rectPTR->m_Atlas = m_Atlas;
		rectPTR->m_Color = m_Color;
		m_ElementList.PushBack(rectPTR);
	}
}


void copium(){
	GHUD::DrawAllocator allocat{};
	allocat.PushBack(new GHUD::Element::Rect());
	for (auto type : allocat) {

	}
}