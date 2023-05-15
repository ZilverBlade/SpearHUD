#pragma once

#include <ghudcpp/draw/draw_list.h>

#define GHUD_ANCHOR_OFFSET_CENTER GHUD::fvec2{ 0.0f, 0.0f }

#define GHUD_ANCHOR_OFFSET_TOP GHUD::fvec2{ 0.0f, 1.0f }
#define GHUD_ANCHOR_OFFSET_BOTTOM GHUD::fvec2{ 0.0f, -1.0f }
#define GHUD_ANCHOR_OFFSET_LEFT GHUD::fvec2{ -1.0f, 0.0f }
#define GHUD_ANCHOR_OFFSET_RIGHT GHUD::fvec2{ 1.0f, 0.0f }

#define GHUD_ANCHOR_OFFSET_TOP_LEFT GHUD::fvec2{ -1.0f, 1.0f }
#define GHUD_ANCHOR_OFFSET_BOTTOM_LEFT  GHUD::fvec2{ -1.0f, -1.0f }
#define GHUD_ANCHOR_OFFSET_TOP_RIGHT GHUD::fvec2{ 1.0f, 1.0f }
#define GHUD_ANCHOR_OFFSET_BOTTOM_RIGHT  GHUD::fvec2{ 1.0f, -1.0f }