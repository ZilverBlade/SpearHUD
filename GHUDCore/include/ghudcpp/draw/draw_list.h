#pragma once

#include <ghudcpp/context.h>
#include <ghudcpp/draw/element.h>

namespace GHUD {
	class DrawList : NonCopyableClass {
	public:
		DrawList();
		inline void Clear() { m_ElementList.clear(); }
		inline const auto& GetList() { return m_ElementList; }

		Element::Rect& DrawRect(const Element::Rect& rect);
	private:
		std::vector<Element::ElementBase*> m_ElementList;
	};
}