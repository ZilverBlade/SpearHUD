#version 450
#extension GL_GOOGLE_include_directive : enable

#include "shared.glsl"

layout (location = 0) in vec2 fragUV;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 1) buffer IDSSBO {
	uint m_PickID;
} idssbo;
layout (set = 1, binding = 0) uniform sampler2D textureAtlas;

void main() {
	vec4 color = vec4(1.0);
	if (push.m_HasTexture == 1) {
		vec2 BminA = push.m_SubUVOffsetB - push.m_SubUVOffsetA;
		vec2 textureCoord = mod(fragUV * BminA, BminA) + push.m_SubUVOffsetA;
		color = textureLod(textureAtlas, textureCoord, 1.0).rgba * push.m_Color;
		if (color.a < 0.01) discard; // masked objects to avoid unintended button presses
	} else {
		color = push.m_Color;
	}
	if (push.m_HasInteraction == 1) 
		if (distance(fragUV, ubo.m_CursorCoord) < 0.00001) 
			idssbo.m_PickID = push.m_ID;
	
	outColor = vec4(pow(color.rgb, vec3(ubo.m_InvGamma)), color.a);
}