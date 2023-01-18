#pragma once

#include <ghudcpp/draw/element.h>
#include <unordered_map>
namespace GHUD {
	template<typename T>
	class ElementIterator {
	public:
		struct ElementInfo {
			T m_Element;
			const size_t m_ID;
		};
		constexpr ElementIterator(T* elementArray, size_t index) : m_ElementArray(elementArray), m_IterIndex(index) {}
		constexpr inline ElementIterator& operator++() noexcept { m_IterIndex++; return *this; }
		inline ElementIterator operator++(int) noexcept { ElementIterator tmp = *this; ++(*this); return tmp; }
		constexpr inline friend bool operator== (const ElementIterator& a, const ElementIterator& b) noexcept { return a.m_IterIndex == b.m_IterIndex; };
		constexpr inline friend bool operator!= (const ElementIterator& a, const ElementIterator& b) noexcept { return a.m_IterIndex != b.m_IterIndex; };
		constexpr inline ElementInfo operator*() const noexcept {
			return { m_ElementArray[m_IterIndex], m_IterIndex };
		}
	private:
		T* m_ElementArray;
		size_t m_IterIndex;
	};

	template <typename T>
	class ElementVector {
	public:
		constexpr inline ElementVector() noexcept {
			m_Elements = reinterpret_cast<T*>(malloc(1));
			m_WholeSize = 1;
		}
		inline ~ElementVector() noexcept {
			this->Clear();
		}
		constexpr inline void PushBack(const T& element) noexcept {
			m_Size++;
			size_t arraySize = m_Size * sizeof(T);
			if (arraySize > m_WholeSize) {
				this->Allocate(4 * sizeof(T));
			}
			m_Elements[m_Size - 1] = element;
		}
		constexpr inline void Allocate(size_t size) noexcept {
			m_WholeSize += size; // allocate enough for 4 future elements
			m_Elements = reinterpret_cast<T*>(realloc(m_Elements, m_WholeSize));
		}
		constexpr inline T& At(size_t index) const noexcept {
			assert(index <= m_Size && "Index out of array bounds");
			return m_Elements[index];
		}
		constexpr inline size_t Size() const noexcept {
			return m_Size;
		}
		constexpr inline T& operator[](size_t i) noexcept {
			return this->At(i);
		}
		constexpr inline void Clear() noexcept {
			free(m_Elements);
			m_Size = 0;
			m_WholeSize = 0;
		}

		inline T& First() noexcept { return m_Elements[0]; }
		inline T& Last() noexcept { return m_Elements[m_Size]; }

		inline ElementIterator<T> begin() const noexcept { return ElementIterator<T>(m_Elements, 0); }
		inline ElementIterator<T> end() const noexcept { return ElementIterator<T>(m_Elements, m_Size); }
	protected:
		size_t m_Size = 0;
		size_t m_WholeSize = 0;
		T* m_Elements{};
	};
}