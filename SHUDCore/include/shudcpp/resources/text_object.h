#pragma once

#include <shudcpp/resources/resource_object.h>

namespace SHUD {
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
			uint32_t flags; 
		//	const char* char_block4; // 16000 max for uniform buffer compatibility
		};
	}

	struct MSDFCharacterOffsetData {
		uint16_t bottomleft_h[2];
		uint16_t topleft_h[2];
		uint16_t bottomright_h[2];
		uint16_t topright_h[2];
	};

	enum class TextFormattingHAlignment {
		Left = 0,
		Center = 1,
		Right = 2,
		//	Justified = 3	:: TODO
	};
	enum class TextFormattingVAlignment {
		Bottom = 0,
		//	Center = 1,		:: TODO
		//	Top = 2			:: TODO
	};

	struct TextFormatting {
		TextFormattingHAlignment mHAlignment = TextFormattingHAlignment::Left;
		TextFormattingVAlignment mVAlignment = TextFormattingVAlignment::Bottom;
		bool mBold = false;
		bool mItalic = false;
		float mOutlineWidth = 0.0f;
		float mSizePx = 16.0f;
		float mLineSpacing = 0.07f;
		RGBAColor mOutlineColor = 0x000000FF;
	};

	const uint32_t FONT_FORMATTING_FLAG_BOLD = 0x01;
	const uint32_t FONT_FORMATTING_FLAG_ITALIC = 0x02;
}