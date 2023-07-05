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
	uint mCharBlock4[ANY_SIZE]; // 4 chars per index
} msdfData;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

vec2 warpUV(vec2 uv, float warpSize) {
	
	return uv;
}

vec4 sampleChar(uint idx, sampler2D msdfAtlas, vec2 UV) {
	if (idx >= msdfData.mCharCount) return vec4(1.0, 0.0, 0.0, 1.0); // out of bounds
	uint char = (msdfData.mCharBlock4[uint(floor(float(idx) * 0.25))] >> ((idx % 4) * 8)) & 0x000000FF;
	MSDFCoord cd = msdfLUT.mUV[char];
	
	UV.y = 1.0 - UV.y;
	vec2 BminA = cd.maxAtlas - cd.minAtlas;
	float m = 1.0 / float(msdfData.mCharCount);
	UV.x = mod(UV.x, m) / m;
	
	vec2 DminC = cd.maxPlane - cd.minPlane;
	//UV = cd.minPlane;
	UV = UV * 2.0 - 1.0;
	UV /= cd.maxPlane;
	UV = 0.5 * (UV + 1.0);
	
	vec2 x = clamp(BminA * (UV) + cd.minAtlas, cd.minAtlas, cd.maxAtlas);
	
	vec3 msdf = textureLod(msdfAtlas, x, 0.0).rgb;//sampleAtlas(msdfAtlas, newV, cd.min, cd.max).rgb;
	
	vec2 sz = textureSize(msdfAtlas, 0).xy;
    float dx = dFdx(UV.x) * sz.x; 
    float dy = dFdy(UV.y) * sz.y;
    float toPixels = 8.0 * inversesqrt(dx * dx + dy * dy);
    float sigDist = median(msdf.r, msdf.g, msdf.b);
    float w = fwidth(sigDist);
    float opacity = smoothstep(0.5 - w, 0.5 + w, sigDist);
	
	return vec4(1.0, 1.0, 1.0, opacity);
	
	//return vec4( msdf, 1.0);
}

uint calcCharIndex(vec2 UV) {
	float block = floor(UV.x * float(msdfData.mCharCount));
	return uint(block);
}

#endif