#pragma once

#include <shudcpp/core.h>
#include <shudcpp/input.h>
#include <unordered_map>=

namespace SHUD {
	struct IO {
		std::vector<int> mCharInputs; // vector of key codes
		std::unordered_map<int, bool> mKeyState; // keyboard state
		std::unordered_map<int, bool> mButtonState; // mouse button state
		fvec2 mCursorPosition;
		fvec2 mCursorDeltaScroll;
	};
}
