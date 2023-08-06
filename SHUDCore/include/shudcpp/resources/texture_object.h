#pragma once

#include <shudcpp/resources/resource_object.h>

namespace SHUD {
	inline namespace Resources {
		struct TexCoordLimits {
			fvec2 mUVOffsetMin = { 0.f, 0.f };
			fvec2 mUVOffsetMax = { 1.f, 1.f };
		};
		struct TextureObject {
			ResourceObject mAtlas{};

			TexCoordLimits mDefaultTextureCoords{};
			TexCoordLimits mSelectTextureCoords{};
			TexCoordLimits mHoverTextureCoords{};
			TexCoordLimits mPressTextureCoords{};
			TexCoordLimits mDisabledTextureCoords{};
		};
	}
}