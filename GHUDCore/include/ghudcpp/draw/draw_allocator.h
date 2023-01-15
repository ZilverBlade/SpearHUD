#pragma once

#include <ghudcpp/draw/element.h>
#include <unordered_map>
namespace GHUD {
	class DrawIterator {
	public:
		constexpr DrawIterator(Element::Base** elementArray, size_t index) : m_ElementArray(elementArray), m_IterIndex(index) {}
		constexpr inline DrawIterator& operator++() { m_IterIndex++; return *this; }
		inline DrawIterator operator++(int) { DrawIterator tmp = *this; ++(*this); return tmp; }
		constexpr inline friend bool operator== (const DrawIterator& a, const DrawIterator& b) { return a.m_IterIndex == b.m_IterIndex; };
		constexpr inline friend bool operator!= (const DrawIterator& a, const DrawIterator& b) { return a.m_IterIndex != b.m_IterIndex; };

		constexpr inline Element::Base* operator*() const {
			return m_ElementArray[m_IterIndex];
		}
	private:
		Element::Base** m_ElementArray;
		size_t m_IterIndex;
	};

	class DrawAllocator {
	public:
		constexpr inline DrawAllocator() {
			m_Elements = reinterpret_cast<Element::Base**>(malloc(1));
			m_WholeSize = 1;
		}
		inline ~DrawAllocator() {
			this->Clear();
		}
		constexpr inline void PushBack(Element::Base* element) {
			m_Size++;
			size_t arraySize = m_Size * sizeof(element);
			if (arraySize > m_WholeSize) {
				this->Allocate(arraySize);
			}
			m_Elements[m_Size - 1] = element;
		}
		constexpr inline void Allocate(size_t size) {
			m_WholeSize += sizeof(Element::Base*);
			m_Elements = reinterpret_cast<Element::Base**>(realloc(m_Elements, m_Size * sizeof(Element::Base*)));
		}
		constexpr inline Element::Base* At(size_t index) const {
			assert(index <= m_Size && "Index out of array bounds");
			return m_Elements[index];
		}
		constexpr inline size_t Size() const {
			return m_Size;
		}
		constexpr inline Element::Base* operator[](size_t i) noexcept {
			return this->At(i);
		}
		constexpr inline  inline void Clear() {
			for (size_t i = 0; i < m_Size; i++) {
				delete m_Elements[i];
			}
			free(m_Elements);
			m_Size = 0;
			m_WholeSize = 0;
		}
		inline DrawIterator begin() { return DrawIterator(m_Elements, 0); }
		inline DrawIterator end() { return DrawIterator(m_Elements, m_Size); }
	private:
		size_t m_Size = 0;
		size_t m_WholeSize = 0;
		Element::Base** m_Elements{};
	};
}