#pragma once

#include <ghudcpp/resources/resource_object.h>

namespace GHUD {
	inline namespace Resources {
		struct TextShaderBuffer : NonCopyableClass{
			virtual const void* Get0() const {
				return mResourcePtr0;
			}
			virtual const void* Get1() const {
				return mResourcePtr1;
			}
			virtual const void* Get2() const {
				return mResourcePtr2;
			}
			virtual void* _Set0(void* void0) {
				mResourcePtr0 = void0;
				return mResourcePtr0;
			}
			virtual void* _Set1(void* void1) {
				mResourcePtr1 = void1;
				return mResourcePtr1;
			}
			virtual void* _Set2(void* void2) {
				mResourcePtr2 = void2;
				return mResourcePtr1;
			}
			virtual	const BufferID GetBufferID() const {
				return reinterpret_cast<BufferID>(mResourcePtr0);
			}
		private:
			void* mResourcePtr0;
			void* mResourcePtr1;
			void* mResourcePtr2;
		};

		// CONVENTION
		struct MSDFTextData {
			float fontSizePx;
			float kerningPx; // TODO: sample texture twice if overlapping
			uint32_t char_count; // must be equal to (extentMax - extentMin) / fontSizeUV 
			uint32_t x; // padding
		//	const char* char_block4; // 16000 max for uniform buffer compatibility
		};

		struct MSDFCharacterOffsetData {
			uint16_t topleft_h[2];
			uint16_t bottomleft_h[2];
			uint16_t topright_h[2];
			uint16_t bottomright_h[2];
		};
	}
}