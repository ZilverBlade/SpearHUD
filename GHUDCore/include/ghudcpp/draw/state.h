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
		PressState m_PressState = PressState::None;
		uint64 m_ElementID = 0;
	};

}