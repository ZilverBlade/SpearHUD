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
		ElementVector() noexcept {
			m_Elements = reinterpret_cast<T*>(malloc(sizeof(T) * 16));
			m_WholeSize = sizeof(T) * 16;
		}
		~ElementVector() noexcept {
			this->Clear();
		}
		constexpr inline void PushBack(const T& element) noexcept {
			size_t memIndex = m_Size * sizeof(T);
			m_Size++;
			size_t arraySize = m_Size * sizeof(T);
			if (arraySize > m_WholeSize) {
				this->Allocate(4 * sizeof(T));
			}
			memcpy(m_Elements + memIndex, &element, sizeof(T));
		}
		constexpr inline void Allocate(size_t size) noexcept {
			m_WholeSize += size; 

			void* new_buff{};
			new_buff = std::realloc(m_Elements, m_WholeSize);
			assert(new_buff && "failed to allocate buffer memory!");
			m_Elements = reinterpret_cast<T*>(new_buff);
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
			m_Elements = reinterpret_cast<T*>(malloc(sizeof(T) * 16));
			m_WholeSize = sizeof(T) * 16;
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