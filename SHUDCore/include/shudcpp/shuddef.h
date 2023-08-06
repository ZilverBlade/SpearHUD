#pragma once

#include <shudcpp/draw/draw_list.h>

#define SHUD_ANCHOR_OFFSET_CENTER SHUD::fvec2{ 0.0f, 0.0f }

#define SHUD_ANCHOR_OFFSET_TOP SHUD::fvec2{ 0.0f, 1.0f }
#define SHUD_ANCHOR_OFFSET_BOTTOM SHUD::fvec2{ 0.0f, -1.0f }
#define SHUD_ANCHOR_OFFSET_LEFT SHUD::fvec2{ -1.0f, 0.0f }
#define SHUD_ANCHOR_OFFSET_RIGHT SHUD::fvec2{ 1.0f, 0.0f }

#define SHUD_ANCHOR_OFFSET_TOP_LEFT SHUD::fvec2{ -1.0f, 1.0f }
#define SHUD_ANCHOR_OFFSET_BOTTOM_LEFT  SHUD::fvec2{ -1.0f, -1.0f }
#define SHUD_ANCHOR_OFFSET_TOP_RIGHT SHUD::fvec2{ 1.0f, 1.0f }
#define SHUD_ANCHOR_OFFSET_BOTTOM_RIGHT  SHUD::fvec2{ 1.0f, -1.0f }