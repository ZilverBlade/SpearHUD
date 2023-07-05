#version 450

#extension GL_GOOGLE_include_directive : enable
#include "shared.glsl"

const vec2 VERTICES[4] = vec2[](
	vec2(-1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, -1.0),
	vec2(1.0, 1.0)
);
const uint INDICES[6] = uint[](
	0, 1, 2, 2, 1, 3
);
layout (location = 0) out vec2 fragUV;


void main() {
	vec2 arTransform = vec2(ubo.mInvAspectRatio, 1.0);
	vec2 invArTransform = vec2(ubo.mAspectRatio, 1.0);
	uint ind = INDICES[gl_VertexIndex];
	vec2 vtx = VERTICES[ind];
	vec2 offsetVTX = vtx - push.mTransformAnchorOffset;
	vec2 transform = vec2(push.mRotationMatrix * offsetVTX) + push.mPosition;
	vec2 position = transform + push.mPositionAnchorOffset; // i want to kms

	vec2 texCoords[4] = vec2[](
		vec2(push.mUVOffsetA.x, push.mUVOffsetB.y),
		vec2(push.mUVOffsetA.x, push.mUVOffsetA.y),
		vec2(push.mUVOffsetB.x, push.mUVOffsetB.y),
		vec2(push.mUVOffsetB.x, push.mUVOffsetA.y)
	);
	
	fragUV = texCoords[ind];
	gl_Position = vec4(position, 1.0, 1.0); // disabled depth test
}