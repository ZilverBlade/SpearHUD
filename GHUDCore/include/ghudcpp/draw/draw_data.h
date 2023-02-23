#pragma once

#include <ghudcpp/types.h>
#include <ghudcpp/utils/math.h>
namespace GHUD {
	struct DrawData {
		Math::Transform2x2 mRotationMatrix;
		fvec2 mPosition;
		int padding[2];
		fvec2 mTransformOffset{};
		fvec2 mAnchorOffset{};
		fvec2 mUVOffsetA{};
		fvec2 mUVOffsetB{};
		fvec2 mSubUVOffsetA{};
		fvec2 mSubUVOffsetB{};
		fvec4 mColor{};
		uint32 mID{};
		uint32 mHasTexture{};
		uint32 mHasInteraction{};
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