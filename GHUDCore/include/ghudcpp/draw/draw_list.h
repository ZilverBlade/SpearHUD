#pragma once

#include <ghudcpp/context.h>
#include <ghudcpp/draw/element.h>
#include <ghudcpp/draw/draw_allocator.h>
#include <unordered_map>
#include <vector>
namespace GHUD {
	class DrawList : virtual public NonCopyableClass {
	public:
		DrawList();
		~DrawList();
		void FrameStart();
		void FrameEnd();
		void Clear();
		inline const auto& GetList() { 
			return m_ElementList; 
		}

		Element::Line& DrawLine(const Element::Line& line);
		Element::Line& DrawLine(fvec2 m_PointA, fvec2 m_PointB, RGBAColor m_Color);
		Element::Rect& DrawRect(const Element::Rect& rect);
		Element::Rect& DrawRect(const Element::Transform& m_Transform, const TextureObject& m_Texture, RGBAColor m_Color);
	private:
		ElementVector<Element::Base*> m_ElementList;
		ElementVector<Element::Base*> m_HeapElements; // helper vector to track heap allocated elements

		ElementVector<Element::Line> m_Lines; // lines must be fast to draw;
		ElementVector<Element::Rect> m_Rects;
		ElementVector<Element::Image> m_Images;
		ElementVector<Element::Text> m_Texts;

		size_t m_PreviousSize = 0;
	};
}