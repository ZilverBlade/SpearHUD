#pragma once

#include <ghudcpp/resources/resource_object.h>

namespace GHUD {
	inline namespace Resources {
		struct TextureObject {
			ResourceObject m_TextureAtlas{};
			fvec2 m_SubSize{ 0.5f, 0.5f };
			uint32 m_GridCountHorizontal = 2;
			uint32 m_GridCountVertical = 4;
		};
	}
}