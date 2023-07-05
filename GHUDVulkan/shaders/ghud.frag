#version 450
#extension GL_GOOGLE_include_directive : enable

#include "shared.glsl"
#include "sampler.glsl"
#include "msdf.glsl"

layout (location = 0) in vec2 fragUV;

layout (location = 0) out vec4 outColor;

//layout (location = 1) in flat vec4 anchors;

layout (std140, set = 0, binding = 1) buffer IDSSBO {
	uint mPickID[2]; // fake uint64 due to performance and compatibility
} idssbo;
layout (set = 1, binding = 0) uniform sampler2D textureAtlas;

void main() {
	vec4 color = unpackCol32(push.mColorCompressed);
	if (bool(push.mFlags & GHUD_DRAW_DATA_FLAG_HAS_TEXTURE)) {
		if (bool(push.mFlags & GHUD_DRAW_DATA_FLAG_IS_MSDF_TEXT)) { 
			uint idx = calcCharIndex(fragUV);
			color *= sampleChar(idx, textureAtlas, fragUV);
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