#pragma once

#include <ghudcpp/resources/resource_object.h>

namespace GHUD {
	inline namespace Resources {
		class Texture {
		public:
			ResourceObject* GetTexture() {}
		private:
			ResourceObject* m_ResourcePtr;
		};
	}
}