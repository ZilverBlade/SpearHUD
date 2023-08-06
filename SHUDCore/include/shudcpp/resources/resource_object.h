#pragma once

#include <shudcpp/core.h>

namespace SHUD {
	inline namespace Resources {
		class ResourceObject {
		public:
			virtual const void* Get() const { return mResourcePtr; }
			virtual void* _Set(void* void_) {
				mResourcePtr = void_;
				return mResourcePtr;
			}
			virtual	const TextureID GetTextureID() const { return reinterpret_cast<TextureID>(mResourcePtr); }
		private:
			void* mResourcePtr;
		};
	}
}