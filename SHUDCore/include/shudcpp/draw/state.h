#pragma once

#include <shudcpp/core.h>

namespace SHUD {

	enum PressStateFlagBits : uint32 {
		SHUD_PRESS_STATE_FLAG_NONE = 0x00,
		SHUD_PRESS_STATE_FLAG_HOVERING = 0x01,
		SHUD_PRESS_STATE_FLAG_CLICKED = 0x02,
		SHUD_PRESS_STATE_FLAG_PRESSED = 0x04,
		SHUD_PRESS_STATE_FLAG_SELECTED = 0x08
	};
	using PressStateFlags = uint32;

	class PressState {
	public:
		PressState() = default;
		PressState(PressStateFlags f) : flags(f) {}
		bool IsHovering() const {
			return flags & SHUD_PRESS_STATE_FLAG_HOVERING;
		}
		bool IsClicked() const {
			return flags & SHUD_PRESS_STATE_FLAG_CLICKED;
		}
		bool IsPressed() const {
			return flags & SHUD_PRESS_STATE_FLAG_PRESSED;
		}
		bool IsSelected() const {
			return flags & SHUD_PRESS_STATE_FLAG_SELECTED;
		}
	private:
		PressStateFlags flags = SHUD_PRESS_STATE_FLAG_NONE;
	};

}