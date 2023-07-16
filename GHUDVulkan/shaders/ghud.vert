#version 450

#extension GL_GOOGLE_include_directive : enable
#include "shared.glsl"
#include "msdf.glsl"

layout (std140, set = 2, binding = 1) uniform Buffer {
	uvec4 offsets[ANY_SIZE]; 
} charOffsets;


const vec2 VERTICES[4] = vec2[](
	vec2(-1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, -1.0),
	vec2(1.0, 1.0)
);
layout (location = 0) out vec2 fragUV;
layout (location = 1) out flat uint msdfCharIdx;

void main() {
	vec2 arTransform = vec2(ubo.mInvAspectRatio, 1.0);
	vec2 invArTransform = vec2(ubo.mAspectRatio, 1.0);
	vec2 vtx;
	if (bool(push.mFlags & GHUD_DRAW_DATA_FLAG_IS_MSDF_TEXT)) {
		vtx = unpackHalf2x16(charOffsets.offsets[gl_InstanceIndex][gl_VertexIndex]);
		msdfCharIdx = getChar(gl_InstanceIndex);
	} else { 
		vtx = VERTICES[gl_VertexIndex];
	}
	
	vec2 offsetVTX = vtx - push.mTransformAnchorOffset;
	vec2 transform = vec2(push.mRotationMatrix * offsetVTX) + push.mPosition;
	vec2 position = transform + push.mPositionAnchorOffset; // i want to kms

	vec2 texCoords[4] = vec2[](
		vec2(push.mUVOffsetA.x, push.mUVOffsetB.y),
		vec2(push.mUVOffsetA.x, push.mUVOffsetA.y),
		vec2(push.mUVOffsetB.x, push.mUVOffsetB.y),
		vec2(push.mUVOffsetB.x, push.mUVOffsetA.y)
	);
	
	fragUV = texCoords[gl_VertexIndex];
	gl_Position = vec4(position, 1.0, 1.0); // disabled depth test
}