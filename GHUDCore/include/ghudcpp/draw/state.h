#pragma once

#include <ghudcpp/core.h>

namespace GHUD {

	enum class PressState {
		None,
		Hovering,
		Clicked,
		Pressed,
		Selected
	};

	struct State {
		PressState mPressState = PressState::None;
		uint64 mElementID = 0;
	};

}