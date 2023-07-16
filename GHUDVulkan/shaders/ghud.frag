#version 450
#extension GL_GOOGLE_include_directive : enable

#include "shared.glsl"
#include "sampler.glsl"
#include "msdf.glsl"

layout (location = 0) in vec2 fragUV;
layout (location = 1) in flat uint msdfCharIdx;

layout (location = 0) out vec4 outColor;

layout (std140, set = 0, binding = 1) buffer IDSSBO {
	uvec2 mPickID; // fake uint64 due to performance and compatibility
} idssbo;
layout (set = 1, binding = 0) uniform sampler2D textureAtlas;


vec4 sampleChar(uint char, sampler2D msdfAtlas, vec2 UV) {
	MSDFCoord cd = msdfLUT.mUV[char];
	
	vec2 x = clamp((cd.maxAtlas - cd.minAtlas) * UV + cd.minAtlas, cd.minAtlas, cd.maxAtlas);
	
	vec3 msdf = textureLod(msdfAtlas, x, 0.0).rgb;
	
	vec2 sz = textureSize(msdfAtlas, 0).xy;
    float dx = dFdx(UV.x) * sz.x; 
    float dy = dFdy(UV.y) * sz.y;
    float toPixels = 8.0 * inversesqrt(dx * dx + dy * dy);
    float sigDist = median(msdf.r, msdf.g, msdf.b);
    float w = fwidth(sigDist);
    float opacity = smoothstep(0.5 - w, 0.5 + w, sigDist);
	
	return vec4(1.0, 1.0, 1.0, opacity);
}

void main() {
	vec4 color = unpackCol32(push.mColorCompressed);
	if (bool(push.mFlags & GHUD_DRAW_DATA_FLAG_HAS_TEXTURE)) {
		if (bool(push.mFlags & GHUD_DRAW_DATA_FLAG_IS_MSDF_TEXT)) {
			color *= sampleChar(msdfCharIdx, textureAtlas, fragUV);
		} else {
			color *= sampleAtlas(textureAtlas, fragUV, push.mSubUVOffsetA, push.mSubUVOffsetB).rgba;
		}
	}
	if (bool(push.mFlags & GHUD_DRAW_DATA_FLAG_HAS_INTERACTION)) {
		if (distance(gl_FragCoord.xy, ubo.mCursorPosition) <= 1.0 && color.a > push.mInteractableAlphaCutoff) {
			idssbo.mPickID[0] = push.mID[0];
			idssbo.mPickID[1] = push.mID[1];
		}
	}
	
	outColor = vec4(pow(color.rgb, vec3(ubo.mInvGamma)), color.a);
}