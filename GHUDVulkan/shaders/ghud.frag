#version 450

layout (location = 0) in vec2 FragUV

layout (location = 0) out vec4 OutColor;

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
	uint m_Color;
	uint m_ID;
	uint m_HasTexture;
	uint m_HasInteraction;
} push;

void main() {


	float red = (push.m_Color & 0xFF000000) / 255;
	float green = (push.m_Color & 0x00FF0000) / 255;
	float blue = (push.m_Color & 0x0000FF00) / 255;
	float alpha = (push.m_Color & 0x000000FF) / 255;


	if (length(FragUV, ubo.m_CursorCoord) < 0.00001) idssbo.m_PickID = push.m_ID;
	
	
}