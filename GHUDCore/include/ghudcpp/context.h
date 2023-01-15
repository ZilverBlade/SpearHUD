#pragma once

#include <ghudcpp/core.h>

namespace GHUD {
class DrawList;
	class Context {
	public:
		virtual void Render(DrawList* drawList) {}
		virtual void Pick(fvec2 mouseCoord) {}
	};
}
