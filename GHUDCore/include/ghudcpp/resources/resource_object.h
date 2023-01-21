#pragma once

#include <ghudcpp/core.h>

namespace GHUD {
	inline namespace Resources {
		class ResourceObject {
		public:
			virtual const void* Get() const { return m_ResourcePtr; }
			virtual void* _Set(void* void_) {
				m_ResourcePtr = void_;
				return m_ResourcePtr;
			}
			virtual	const TextureID GetTextureID() const { return reinterpret_cast<TextureID>(m_ResourcePtr); }
		private:
			void* m_ResourcePtr;
		};
	}
}