#ifndef SHARED_GLSL
#define SHARED_GLSL
// contents are shared between vertex and fragment shader

layout (set = 0, binding = 0) uniform GlobalUBO {
	vec2 m_CursorCoord;
	vec2 m_ScreenSize;
	float m_AspectRatio;
	float m_InvAspectRatio;
	float m_Gamma;
	float m_InvGamma;
} ubo;

layout (push_constant) uniform Push {
	mat2 m_RotationMatrix;
	vec2 m_Position;
	vec2 m_AnchorOffset;
	vec2 m_UVOffsetA;
	vec2 m_UVOffsetB;
	vec2 m_SubUVOffsetA;
	vec2 m_SubUVOffsetB;
	vec4 m_Color;
	uint m_ID;
	uint m_HasTexture;
	uint m_HasInteraction;
} push;
#endif