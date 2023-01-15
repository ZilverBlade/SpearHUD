#pragma once

#include <ghudcpp/context.h>
#include <ghudcpp/draw/element.h>
#include <ghudcpp/draw/draw_allocator.h>
#include <unordered_map>
namespace GHUD {
	class DrawList : public NonCopyableClass {
	public:
		DrawList();
		~DrawList();
		void FrameStart();
		void FrameEnd();
		inline void Clear() {
			m_PreviousSize = m_ElementList.Size(); 
			m_ElementList.Clear(); }
		inline const auto& GetList() { 
			return m_ElementList; 
		}

		Element::Rect& DrawRect(const Element::Rect& rect);
		Element::Rect& DrawRect(const Element::Transform& m_Transform, TextureObject m_Atlas, RGBAColor m_Color);
	private:
		DrawAllocator m_ElementList;
		size_t m_PreviousSize = 0;
	};
}