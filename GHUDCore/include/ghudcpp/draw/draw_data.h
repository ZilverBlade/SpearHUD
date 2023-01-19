#pragma once

#include <ghudcpp/types.h>
#include <ghudcpp/utils/math.h>
namespace GHUD {
	struct DrawData {
		fvec2 m_RotationMatrix[2];
		fvec2 m_Position;
		fvec2 m_AnchorOffset;
		fvec2 m_UVOffsetA{};
		fvec2 m_UVOffsetB{};
		fvec4 m_Color{};
		uint32 m_ID{};
		uint32 m_HasTexture{};
		uint32 m_HasInteraction{};
	};
	struct DrawInfo {
		LayerIndex m_Layer;
		TextureID m_TextureID;
		DrawData* m_Data;

		// for the std::multi_set
		inline bool operator<(const DrawInfo& other) const {
			return m_Layer < other.m_Layer;
		}
		inline bool operator>(const DrawInfo& other) const {
			return m_Layer > other.m_Layer;
		}
	};
}