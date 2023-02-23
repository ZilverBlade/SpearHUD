#version 450
#extension GL_GOOGLE_include_directive : enable

#include "shared.glsl"

layout (location = 0) in vec2 fragUV;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 1) buffer IDSSBO {
	uint mPickID;
} idssbo;
layout (set = 1, binding = 0) uniform sampler2D textureAtlas;

void main() {
	vec4 color = vec4(1.0);
	if (push.mHasTexture == 1) {
		vec2 BminA = push.mSubUVOffsetB - push.mSubUVOffsetA;
		vec2 textureCoord = mod(fragUV * BminA, BminA) + push.mSubUVOffsetA;
		color = textureLod(textureAtlas, textureCoord, 1.0).rgba * push.mColor;
		if (color.a < 0.01) discard; // masked objects to avoid unintended button presses
	} else {
		color = push.mColor;
	}
	if (push.mHasInteraction == 1) 
		if (distance(fragUV, ubo.mCursorCoord) < 0.00001) 
			idssbo.mPickID = push.mID;
	
	outColor = vec4(pow(color.rgb, vec3(ubo.mInvGamma)), color.a);
}