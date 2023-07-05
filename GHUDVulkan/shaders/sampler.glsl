#ifndef SAMPLER_GLSL
#define SAMPLER_GLSL

vec4 sampleAtlas(sampler2D atlas, vec2 UV, vec2 minUV, vec2 maxUV) {
	vec2 BminA = maxUV - minUV;
	vec2 textureCoord = mod(UV * BminA, BminA) + minUV;
	return textureLod(atlas, textureCoord, 0.0).rgba;
}

#endif