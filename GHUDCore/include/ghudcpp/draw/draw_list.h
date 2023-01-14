#pragma once

#include <ghudcpp/context.h>
#include <ghudcpp/draw/element.h>
#include <unordered_map>
namespace GHUD {
	class DrawAllocator {
	public:
		void PushBack(Element::ElementBase* element) {
			
		}
		void Clear() {
			for (size_t i = 0; i < m_Size; i++) {
				free(m_Elements[i]);
			}
			free(m_Elements);
		}
	private:
		size_t m_Size;
		Element::ElementBase** m_Elements{};
	};

	class DrawList : NonCopyableClass {
	public:
		DrawList();
		inline void Clear() { m_ElementList.Clear(); }
		inline const auto& GetList() { return m_ElementList; }

		Element::Rect& DrawRect(const Element::Rect& rect);
		Element::Rect& DrawRect(const Element::Transform& m_Transform, TextureObject m_Atlas, RGBAColor m_Color);
	private:
		DrawAllocator m_ElementList;
	};
}