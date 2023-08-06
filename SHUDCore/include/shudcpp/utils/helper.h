#pragma once

#include <shudcpp/types.h>
#include <cstdarg>

namespace SHUD {
    namespace Utils {
        static inline fvec2 ConvertScreenCoordToGPUCoord(fvec2 coord, fvec2 invResolution) {
            return fvec2(coord.x * invResolution.x, -coord.y * invResolution.y);
        }
        static inline fvec2 ConvertPixelScaleToUVScale(fvec2 scalepx, fvec2 invResolution) {
            return scalepx * invResolution;
        }
        static inline fvec2 FlipYCoord(fvec2 coord) {
            return fvec2(coord.x, -coord.y);
        }
        static inline fvec4 ConvertHexColorToFloat4Color(RGBAColor color) {           
            return fvec4(
                float(color.r) / 255.0f,
                float(color.g) / 255.0f,
                float(color.b) / 255.0f,
                float(color.a) / 255.0f
            );
        }
        static inline fvec2 GetAnchorCorrectedPosition(fvec2 absoluteCoord, fvec2 resolution, fvec2 anchorOffset) {
            return ((absoluteCoord / resolution) - 0.5f + anchorOffset) * resolution * 2.f;
        }

        // Unfortunately due to the nature of endianness in CPU registers, a simple reinterpret_cast<uint32&>() wont work as the memory laid out may be reversed
        static uint32 RGBAColorToUnsigned32(RGBAColor color) {
            uint32 out = 0;
            out =
                static_cast<uint32>(color.r) << 24 |
                static_cast<uint32>(color.g) << 16 |
                static_cast<uint32>(color.b) << 8 |
                static_cast<uint32>(color.a);
            return out;
        }
    };
}