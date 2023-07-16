#ifndef MSDF_GLSL
#define MSDF_GLSL

#include "shared.glsl"

struct MSDFCoord {
	vec2 minAtlas;
	vec2 maxAtlas;
	vec2 minPlane;
	vec2 maxPlane;
};
layout (std140, set = 0, binding = 2) uniform MSDFLookupTable {
	MSDFCoord mUV[128]; // ASCII character set
} msdfLUT;
layout (std140, set = 2, binding = 0) uniform MSDFData {
	float mFontSizePx;
	float mKerningPx; // TODO: sample texture twice if overlapping
	uint mCharCount;
	uint padding;
	uvec4 mCharBlock4[ANY_SIZE]; // 16 chars per index
} msdfData;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

vec2 warpUV(vec2 uv, float warpSize) {
	return uv;
}

uint getChar(uint idx) {
	if (idx >= msdfData.mCharCount) return 0; // out of bounds
	uint arrayBlock = uint(floor(float(idx) / 16.0));
	uint vecIndex = (uint(floor(float(idx) / 4.0)) % 4);
	uint byteIndex = (idx % 4) * 0x00000008;
	
	return (msdfData.mCharBlock4[arrayBlock][vecIndex] >> byteIndex) & 0x000000FF;
}


//uint calcCharIndex(vec2 UV) {
//	float block = floor(UV.x * float(msdfData.mCharCount));
//	return uint(block);
//}

#endif