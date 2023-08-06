#ifndef SHARED_GLSL
#define SHARED_GLSL
// contents are shared between vertex and fragment shader

const uint ANY_SIZE = 1;

layout (std140, set = 0, binding = 0) uniform GlobalUBO {
	vec2 mCursorPosition;
	
	vec2 mResolution;
	vec2 mInvResolution;
	float mAspectRatio;
	float mInvAspectRatio;
	
	float mGamma;
	float mInvGamma;
} ubo;

layout (std430, push_constant) uniform Push {
	mat2 mRotationMatrix;
	vec2 mPosition;
	vec2 mTransformAnchorOffset;
	vec2 mPositionAnchorOffset;
	vec2 mUVOffsetA; // min
	vec2 mUVOffsetB; // max
	vec2 mSubUVOffsetA; // min atlas
	vec2 mSubUVOffsetB; // max atlas
	uint mColorCompressed;
	uint mFlags;
	uvec2 mID; // avoid uint64 due to performance and compatibility
	float mInteractableAlphaCutoff;
} push;

//enum DrawDataFlagBits : uint32 
const uint GHUD_DRAW_DATA_FLAG_NONE = 0x00;
const uint GHUD_DRAW_DATA_FLAG_HAS_TEXTURE = 0x01;
const uint GHUD_DRAW_DATA_FLAG_HAS_INTERACTION = 0x02;
const uint GHUD_DRAW_DATA_FLAG_IS_MSDF_TEXT = 0x04;
const uint GHUD_DRAW_DATA_FLAG_SAMPLER_FILTER_MODE_NEAREST = 0x08;
const uint GHUD_DRAW_DATA_FLAG_SAMPLER_FILTER_MODE_LINAER = 0x10;
const uint GHUD_DRAW_DATA_FLAG_SAMPLER_ADDRESS_MODE_REPEAT = 0x20;
const uint GHUD_DRAW_DATA_FLAG_SAMPLER_ADDRESS_MODE_CLAMP = 0x40;
const uint GHUD_DRAW_DATA_FLAG_SAMPLER_ADDRESS_MODE_REPEAT_MIRROR = 0x80;

vec4 unpackCol32(uint cmp) {
	return vec4(
		float((cmp & 0xFF000000) >> 24) / 255.0,
		float((cmp & 0x00FF0000) >> 16) / 255.0,
		float((cmp & 0x0000FF00) >> 8) / 255.0,
		float((cmp & 0x000000FF) >> 0) / 255.0
	);
}

#endif