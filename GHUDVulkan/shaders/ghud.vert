#version 450

const vec2 VERTICES[4] = vec2[](
	vec2(-0.5, -0.5),
	vec2(-0.5, 0.5),
	vec2(0.5, -0.5),
	vec2(0.5, 0.5)
);
const vec2 TEXCOORDS[4] = vec2[](
	vec2(1.0, 1.0),
	vec2(1.0, 0.0),
	vec2(0.0, 1.0),
	vec2(0.0, 0.0)
);
const uint INDICES[6] = uint[](
	0, 1, 2, 2, 1, 3
);

layout (location = 0) out vec2 FragUV
layout (set = 0, binding = 0) uniform GlobalUBO {
	float m_Gamma;
	float m_InvGamma;
	vec2 m_ScreenSize;
	float m_AspectRatio;
	vec2 m_CursorCoord;
} ubo;

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
	uint ind = INDICES[gl_VertexIndex];
	
	vec2 position = vec2(push.m_RotationMatrix * (VERTICES[ind] + push.m_AnchorOffset) + push.m_Position);
	position.y *= ubo.m_AspectRatio;
	FragUV = TEXCOORDS[INDICES[ind]];
	gl_Position = vec4(position, 0.0, 1.0); // disabled depth test
}
