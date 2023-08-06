#version 450
#extension GL_GOOGLE_include_directive : enable

#include "shared.glsl"
#include "sampler.glsl"
#include "msdf.glsl"

layout (location = 0) in vec2 fragUV;
layout (location = 1) in flat uint msdfCharIdx;

layout (location = 0) out vec4 outColor;

layout (std140, set = 0, binding = 1) buffer IDSSBO {
	uvec2 mPickID; // fake uint64 due to performance and compatibility
} idssbo;
layout (set = 1, binding = 0) uniform sampler2D textureAtlas;


float applyFontWidth(float w, float sigDist, float width) {
	float smoothC = smoothstep(0.5 - w, 0.5 + w, sigDist + width);
	return smoothC ;
}
vec3 applyFontOutline(vec3 baseCol, vec3 outlineCol, float sigDist) {
	if (sigDist < 0.5 && sigDist > FONT_OUTLINE_FACTOR) {
		return outlineCol;
	} else {
		return baseCol;
	}
}

vec4 sampleChar(uint char, uint flags, sampler2D msdfAtlas, vec2 UV) {
	
	bool bold = bool(flags & FONT_FORMATTING_FLAG_BOLD);
	bool underline = true;
	bool strike = false;
	bool outline = false;
	
	MSDFCoord cd = msdfLUT.mUV[char];
	UV.y = 1.0 - UV.y;
	vec2 x = clamp((cd.maxAtlas - cd.minAtlas) * UV + cd.minAtlas, cd.minAtlas, cd.maxAtlas);
	
	vec3 msdf = textureLod(msdfAtlas, x, 0.0).rgb;
	
	vec2 sz = textureSize(msdfAtlas, 0).xy;
    float dx = dFdx(UV.x) * sz.x; 
    float dy = dFdy(UV.y) * sz.y;
    float toPixels = 8.0 * inversesqrt(dx * dx + dy * dy);
    float sigDist = median(msdf.r, msdf.g, msdf.b);
    float w = fwidth(sigDist);
    float opacity = applyFontWidth(w, sigDist, bold ? FONT_BOLD_FACTOR : 0.0); 
	
	vec3 color = vec3(1.0, 1.0, 1.0);
	if (outline) {
		color = applyFontOutline(color, vec3(1.0, 0.0, 1.0), sigDist);
	}
	if (underline) {
		
	}
	
	return vec4(color, opacity);
}

void main() {
	vec4 color = unpackCol32(push.mColorCompressed);
	if (bool(push.mFlags & GHUD_DRAW_DATA_FLAG_HAS_TEXTURE)) {
		if (bool(push.mFlags & GHUD_DRAW_DATA_FLAG_IS_MSDF_TEXT)) {
			color *= sampleChar(msdfCharIdx, msdfData.mFlags, textureAtlas, fragUV);
		} else {
			color *= sampleAtlas(textureAtlas, fragUV, push.mSubUVOffsetA, push.mSubUVOffsetB).rgba;
		}
	}
	if (bool(push.mFlags & GHUD_DRAW_DATA_FLAG_HAS_INTERACTION)) {
		if (distance(gl_FragCoord.xy, ubo.mCursorPosition) <= 1.0 && color.a > push.mInteractableAlphaCutoff) {
			idssbo.mPickID[0] = push.mID[0];
			idssbo.mPickID[1] = push.mID[1];
		}
	}
	
	outColor = vec4(pow(color.rgb, vec3(ubo.mInvGamma)), color.a);
}