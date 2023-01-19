#version 450

layout (location = 0) in vec2 fragUV;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform GlobalUBO {
	float m_Gamma;
	float m_InvGamma;
	vec2 m_ScreenSize;
	float m_AspectRatio;
	vec2 m_CursorCoord;
} ubo;
layout (set = 0, binding = 1) buffer IDSSBO {
	uint m_PickID;
} idssbo;
layout (set = 1, binding = 0) uniform sampler2D textureAtlas;

layout (push_constant) uniform Push {
	mat2 m_RotationMatrix;
	vec2 m_Position;
	vec2 m_AnchorOffset;
	vec2 m_UVOffsetA;
	vec2 m_UVOffsetB;
	vec4 m_Color;
	uint m_ID;
	uint m_HasTexture;
	uint m_HasInteraction;
} push;

void main() {
	if (push.m_HasInteraction == 1) 
		if (distance(fragUV, ubo.m_CursorCoord) < 0.00001) 
			idssbo.m_PickID = push.m_ID;
	
	vec4 color = push.m_Color;
	
	if (push.m_HasTexture == 1)
		color *= textureLod(textureAtlas, fragUV, 1.0);
	
	outColor = vec4(pow(color.rgb, vec3(ubo.m_InvGamma)), color.a);
}