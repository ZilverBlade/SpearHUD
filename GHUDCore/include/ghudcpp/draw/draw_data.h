#pragma once

#include <ghudcpp/types.h>
#include <ghudcpp/utils/math.h>
namespace GHUD {
	struct DrawData {
<<<<<<< HEAD
		fvec2 m_RotationMatrix[2];
		fvec2 m_Position;
		int padding[2];
		fvec2 m_TransformOffset{};
		fvec2 m_AnchorOffset{};
		fvec2 m_UVOffsetA{};
		fvec2 m_UVOffsetB{};
		fvec2 m_SubUVOffsetA{};
		fvec2 m_SubUVOffsetB{};
		fvec4 m_Color{};
		uint32 m_ID{};
		uint32 m_HasTexture{};
		uint32 m_HasInteraction{};
=======
		Math::Transform2x2 mRotationMatrix;
		fvec2 mPosition;
		fvec2 mAnchorOffset;
		fvec2 mUVOffsetA{};
		fvec2 mUVOffsetB{};
		fvec2 mSubUVOffsetA{};
		fvec2 mSubUVOffsetB{};
		fvec4 mColor{};
		uint32 mID{};
		uint32 mHasTexture{};
		uint32 mHasInteraction{};
>>>>>>> 2c9f03cdecc6f6ddbddb97a93cad5aa7df7af018
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