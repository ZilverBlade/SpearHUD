#pragma once

#include <ghudcpp/core.h>

namespace GHUD {

	enum PressStateFlagBits : uint32 {
		GHUD_PRESS_STATE_FLAG_NONE = 0x00,
		GHUD_PRESS_STATE_FLAG_HOVERING = 0x01,
		GHUD_PRESS_STATE_FLAG_CLICKED = 0x02,
		GHUD_PRESS_STATE_FLAG_PRESSED = 0x04,
		GHUD_PRESS_STATE_FLAG_SELECTED = 0x08
	};
	using PressStateFlags = uint32;

	class PressState {
	public:
		PressState(PressStateFlags f) : flags(f) {}
		bool IsHovering() const {
			return flags & GHUD_PRESS_STATE_FLAG_HOVERING;
		}
		bool IsClicked() const {
			return flags & GHUD_PRESS_STATE_FLAG_CLICKED;
		}
		bool IsPressed() const {
			return flags & GHUD_PRESS_STATE_FLAG_PRESSED;
		}
		bool IsSelected() const {
			return flags & GHUD_PRESS_STATE_FLAG_SELECTED;
		}
	private:
		PressStateFlags flags;
	};

}