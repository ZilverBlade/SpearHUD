#pragma once

#include <shudcpp/draw/element.h>
#include <unordered_map>
namespace SHUD {
	template<typename T>
	class ElementIterator {
	public:
		struct ElementInfo {
			T mElement;
			const size_t mID;
		};
		constexpr ElementIterator(T* elementArray, size_t index) : mElementArray(elementArray), mIterIndex(index) {}
		constexpr inline ElementIterator& operator++() noexcept { mIterIndex++; return *this; }
		inline ElementIterator operator++(int) noexcept { ElementIterator tmp = *this; ++(*this); return tmp; }
		constexpr inline friend bool operator== (const ElementIterator& a, const ElementIterator& b) noexcept { return a.mIterIndex == b.mIterIndex; };
		constexpr inline friend bool operator!= (const ElementIterator& a, const ElementIterator& b) noexcept { return a.mIterIndex != b.mIterIndex; };
		constexpr inline ElementInfo operator*() const noexcept {
			return { mElementArray[mIterIndex], mIterIndex };
		}
	private:
		T* mElementArray;
		size_t mIterIndex;
	};

	template <typename T>
	class ElementVector {
	public:
		ElementVector() noexcept {
			mElements = reinterpret_cast<T*>(malloc(sizeof(T) * 16));
			mWholeSize = sizeof(T) * 16;
		}
		~ElementVector() noexcept {
			this->Clear();
		}
		constexpr inline void PushBack(const T& element) noexcept {
			size_t memIndex = mSize * sizeof(T);
			mSize++;
			size_t arraySize = mSize * sizeof(T);
			if (arraySize > mWholeSize) {
				this->Allocate(4 * sizeof(T));
			}
			memcpy(mElements + memIndex, &element, sizeof(T));
		}
		constexpr inline void Allocate(size_t size) noexcept {
			mWholeSize += size; 

			void* new_buff{};
			new_buff = std::realloc(mElements, mWholeSize);
			assert(new_buff && "failed to allocate buffer memory!");
			mElements = reinterpret_cast<T*>(new_buff);
		}
		constexpr inline T& At(size_t index) const noexcept {
			assert(index <= mSize && "Index out of array bounds");
			return mElements[index];
		}
		constexpr inline size_t Size() const noexcept {
			return mSize;
		}
		constexpr inline T& operator[](size_t i) noexcept {
			return this->At(i);
		}
		constexpr inline void Clear() noexcept {
			free(mElements);
			mSize = 0;
			mElements = reinterpret_cast<T*>(malloc(sizeof(T) * 16));
			mWholeSize = sizeof(T) * 16;
		}

		inline T& First() noexcept { return mElements[0]; }
		inline T& Last() noexcept { return mElements[mSize]; }

		inline ElementIterator<T> begin() const noexcept { return ElementIterator<T>(mElements, 0); }
		inline ElementIterator<T> end() const noexcept { return ElementIterator<T>(mElements, mSize); }
	protected:
		size_t mSize = 0;
		size_t mWholeSize = 0;
		T* mElements{};
	};
}