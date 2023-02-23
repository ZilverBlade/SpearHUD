#ifndef SHARED_GLSL
#define SHARED_GLSL
// contents are shared between vertex and fragment shader

layout (set = 0, binding = 0) uniform GlobalUBO {
	vec2 mCursorCoord;
	vec2 mScreenSize;
	float mAspectRatio;
	float mInvAspectRatio;
	float mGamma;
	float mInvGamma;
} ubo;

layout (push_constant) uniform Push {
	mat2 mRotationMatrix;
	vec2 mPosition;
	int padding[2];
	vec2 mTransformAnchorOffset;
	vec2 mPositionAnchorOffset;
	vec2 mUVOffsetA; // min
	vec2 mUVOffsetB; // max
	vec2 mSubUVOffsetA; // min atlas
	vec2 mSubUVOffsetB; // max atlas
	vec4 mColor;
	uint mID;
	uint mHasTexture;
	uint mHasInteraction;
} push;
#endif