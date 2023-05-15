#pragma once

#include <ghudcpp/types.h>
#include <ghudcpp/utils/math.h>
namespace GHUD {
	enum DrawDataFlagBits : uint32 {
		GHUD_DRAW_DATA_FLAG_NONE = 0,
		GHUD_DRAW_DATA_FLAG_HAS_TEXTURE = 0x01,
		GHUD_DRAW_DATA_FLAG_HAS_INTERACTION = 0x02,
		GHUD_DRAW_DATA_FLAG_SAMPLER_FILTER_MODE_NEAREST = 0x04,
		GHUD_DRAW_DATA_FLAG_SAMPLER_FILTER_MODE_LINAER = 0x08,
		GHUD_DRAW_DATA_FLAG_SAMPLER_ADDRESS_MODE_REPEAT = 0x10,
		GHUD_DRAW_DATA_FLAG_SAMPLER_ADDRESS_MODE_CLAMP = 0x20,
		GHUD_DRAW_DATA_FLAG_SAMPLER_ADDRESS_MODE_REPEAT_MIRROR = 0x40
	};

	struct DrawData {
		Math::Transform2x2 mRotationMatrix;
		int padd[2];
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
		DrawData mData;

		// for the std::multi_set
		inline bool operator<(const DrawInfo& other) const {
			return mLayer < other.mLayer;
		}
		inline bool operator>(const DrawInfo& other) const {
			return mLayer > other.mLayer;
		}
	};
}