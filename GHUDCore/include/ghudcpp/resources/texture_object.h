#pragma once

#include <ghudcpp/resources/resource_object.h>

namespace GHUD {
	inline namespace Resources {
		struct TexCoordLimits {
			fvec2 m_UVOffsetMin = { 0.f, 0.f };
			fvec2 m_UVOffsetMax = { 1.f, 1.f };
		};
		struct TextureObject {
			ResourceObject m_Atlas{};

			TexCoordLimits m_DefaultTextureCoords{};
			TexCoordLimits m_SelectTextureCoords{};
			TexCoordLimits m_HoverTextureCoords{};
			TexCoordLimits m_PressTextureCoords{};
			TexCoordLimits m_DisabledTextureCoords{};
		};
	}
}