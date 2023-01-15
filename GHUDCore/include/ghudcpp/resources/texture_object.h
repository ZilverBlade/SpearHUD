#pragma once

#include <ghudcpp/resources/resource_object.h>

namespace GHUD {
	inline namespace Resources {
		struct TextureObject {
			ResourceObject m_TextureAtlas{};
			fvec2 m_SubSize{ 0.5f, 0.5f };
			uint32 m_GridCountHorizontal = 2;
			uint32 m_GridCountVertical = 4;
			fvec2 m_UVOffsetA = { 0.f, 0.f };
			fvec2 m_UVOffsetB = { 1.f, 1.f };
		};
	}
}