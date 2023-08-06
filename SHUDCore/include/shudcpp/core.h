#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <shudcpp/platform.h>
#include <shudcpp/types.h>

namespace SHUD {
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
