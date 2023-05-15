#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/input.h>
#include <unordered_map>=

namespace GHUD {
	struct IO {
		std::vector<int> mCharInputs; // vector of key codes
		std::unordered_map<int, bool> mKeyState; // keyboard state
		std::unordered_map<int, bool> mButtonState; // mouse button state
		fvec2 mCursorPosition;
		fvec2 mCursorDeltaScroll;
	};
}
