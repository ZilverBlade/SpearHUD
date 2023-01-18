#pragma once

#include <ghudcpp/types.h>

namespace GHUD {
    namespace Utils {
        static inline fvec2 ConvertScreenCoordToGPUCoord(fvec2 coord) {
            return fvec2(coord.x - 0.5, coord.y - 0.5) * 2.0f;
        }
    };
}