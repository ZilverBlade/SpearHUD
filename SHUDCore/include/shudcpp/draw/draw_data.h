#pragma once

#include <shudcpp/types.h>
#include <shudcpp/utils/math.h>
namespace SHUD {
	enum DrawDataFlagBits : uint32 {
		SHUD_DRAW_DATA_FLAG_NONE = 0x00,
		SHUD_DRAW_DATA_FLAG_HAS_TEXTURE = 0x01,
		SHUD_DRAW_DATA_FLAG_HAS_INTERACTION = 0x02,
		SHUD_DRAW_DATA_FLAG_IS_MSDF_TEXT = 0x04,
		SHUD_DRAW_DATA_FLAG_SAMPLER_FILTER_MODE_NEAREST = 0x08,
		SHUD_DRAW_DATA_FLAG_SAMPLER_FILTER_MODE_LINAER = 0x10,
		SHUD_DRAW_DATA_FLAG_SAMPLER_ADDRESS_MODE_REPEAT = 0x20,
		SHUD_DRAW_DATA_FLAG_SAMPLER_ADDRESS_MODE_CLAMP = 0x40,
		SHUD_DRAW_DATA_FLAG_SAMPLER_ADDRESS_MODE_REPEAT_MIRROR = 0x80
	};

	struct DrawData {
		Math::Transform2x2 mRotationMatrix;
		fvec2 mPosition;
		fvec2 mTransformOffset{};
		fvec2 mAnchorOffset{};
		fvec2 mUVOffsetA{};
		fvec2 mUVOffsetB{};
		fvec2 mSubUVOffsetA{};
		fvec2 mSubUVOffsetB{};
		uint32 mColorCompressed{};
		uint32 mFlags{};
		uint64 mID{}; // on gpu should be array of size 2 of uint32 due to performance and compatibility
		float mInteractableAlphaCutoff{0.5f};

	};
	struct DrawInfo {
		LayerIndex mLayer;
		TextureID mTextureID;
		BufferID mBufferID;
		DrawData mData;
		uint32_t mInstanceCount;

		// for the std::multi_set
		inline bool operator<(const DrawInfo& other) const {
			return mLayer < other.mLayer;
		}
		inline bool operator>(const DrawInfo& other) const {
			return mLayer > other.mLayer;
		}
	};
}