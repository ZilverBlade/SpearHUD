#ifndef MSDF_GLSL
#define MSDF_GLSL

#include "shared.glsl"

const float FONT_BOLD_FACTOR = 0.33;
const float FONT_ITALICS_FACTOR = 0.2;
const float FONT_OUTLINE_FACTOR = 0.45;
const float FONT_UNDERLINE_WIDTH_FACTOR = 0.1;
const float FONT_UNDERLINE_HEIGHT_FACTOR = 0.25;

const uint FONT_FORMATTING_FLAG_BOLD = 0x01;
const uint FONT_FORMATTING_FLAG_ITALIC = 0x02;

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
	float mKerningPx; 
	uint mCharCount;
	uint mFlags;
	uvec4 mCharBlock4[ANY_SIZE]; // 16 chars per index
} msdfData;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
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