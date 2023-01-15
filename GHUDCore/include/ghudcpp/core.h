#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <ghudcpp/platform.h>
#include <ghudcpp/types.h>

namespace GHUD {
	class NonCopyableClass {
	public:
		NonCopyableClass() = default;
		NonCopyableClass(const NonCopyableClass&) = delete;
		NonCopyableClass& operator =(const NonCopyableClass&) = delete;
	};
	class NonMovableClass {
	public:
		NonMovableClass(NonMovableClass&&) = delete;
		NonMovableClass&& operator =(NonMovableClass&&) = delete;
	};
}
