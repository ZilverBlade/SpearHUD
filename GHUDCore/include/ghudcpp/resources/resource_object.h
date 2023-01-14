#pragma once

#include <ghudcpp/core.h>

namespace GHUD {
	inline namespace Resources {
		class ResourceObject {
		public:
			virtual const void* Get() const { return m_ResourcePtr; }
		private:
			void* m_ResourcePtr;
		};
	}
}