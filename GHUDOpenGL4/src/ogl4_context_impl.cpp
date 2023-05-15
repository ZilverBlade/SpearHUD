#include <ghudgl4/gl_context.h>
#include <ghudcpp/draw/draw_data.h>

namespace GHUD {

#pragma region Shader Code
	static const char* vshCode = "															\
		#version 400																		\
																							\
		const vec2 VERTICES[4] = vec2[](													\
			vec2(-1.0, -1.0),																\
			vec2(-1.0, 1.0),																\
			vec2(1.0, -1.0),																\
			vec2(1.0, 1.0)																	\
		);																					\
																							\
		const uint INDICES[6] = uint[](														   \
			0, 1, 2, 2, 1, 3																   \
		);																					   \
																							   \
		layout(location = 0) out vec2 fragUV;												   \
																							   \
		uniform GlobalUBO{																	   \
			vec2 m_CursorCoord;																   \
			vec2 m_ScreenSize;																   \
			float m_AspectRatio;															   \
			float m_InvAspectRatio;															   \
			float m_Gamma;																	   \
			float m_InvGamma;																   \
		} ubo;																				   \
																							   \
		uniform Push{																		   \
			mat2 m_RotationMatrix;															   \
			vec2 m_Position;																   \
			vec2 m_AnchorOffset;															   \
			vec2 m_UVOffsetA; // min														   \
			vec2 m_UVOffsetB; // max														   \
			vec2 m_SubUVOffsetA; // min atlas												   \
			vec2 m_SubUVOffsetB; // max atlas												   \
			vec4 m_Color;																	   \
			uint m_ID;																		   \
			uint m_HasTexture;																   \
			uint m_HasInteraction;															   \
		} push;																				   \
																							   \
		void main() {																		   \
			vec2 arTransform = vec2(ubo.m_InvAspectRatio, 1.0);								   \
			vec2 invArTransform = vec2(ubo.m_AspectRatio, 1.0);								   \
			uint ind = INDICES[gl_VertexIndex];												   \
			vec2 vtx = (VERTICES[ind]);														   \
			vec2 offsetVTX = vtx - push.m_AnchorOffset;										   \
			vec2 transform = vec2(push.m_RotationMatrix * offsetVTX) + push.m_Position;		   \
			vec2 position = transform * arTransform + push.m_AnchorOffset;					   \
																							   \
			vec2 texCoords[4] = vec2[](														   \
				vec2(push.m_UVOffsetA.x, push.m_UVOffsetB.y),								   \
				vec2(push.m_UVOffsetA.x, push.m_UVOffsetA.y),								   \
				vec2(push.m_UVOffsetB.x, push.m_UVOffsetB.y),								   \
				vec2(push.m_UVOffsetB.x, push.m_UVOffsetA.y)								   \
			);																				   \
																							   \
			fragUV = texCoords[ind];														   \
			gl_Position = vec4(position, 1.0, 1.0); // disabled depth test					   \
		}																					   \
																								\
		";
	static const char* fshCode = "";
#pragma endregion

	OpenGL4Context::OpenGL4Context(const OpenGL4ContextCreateInfo& createInfo) {	
		CreateGraphicsProgram(createInfo);
	}

	OpenGL4Context::~OpenGL4Context() {
		glDeleteProgram(m_GraphicsProgram);
	}

	void OpenGL4Context::CreateGraphicsProgram(const OpenGL4ContextCreateInfo& createInfo) {
		GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vsh, 1, &vshCode, nullptr);
		glCompileShader(vsh);

		GLuint fsh = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(fsh, 1, &fshCode, nullptr);
		glCompileShader(fsh);

		m_GraphicsProgram = glCreateProgram();
		glAttachShader(m_GraphicsProgram, vsh);
		glAttachShader(m_GraphicsProgram, fsh);
		glLinkProgram(m_GraphicsProgram);

		glDetachShader(m_GraphicsProgram, vsh);
		glDetachShader(m_GraphicsProgram, fsh);

		glDeleteShader(vsh);
		glDeleteShader(fsh);
	}

	void OpenGL4Context::Render(const OpenGLFrameInfoStruct* frameInfoStruct) {
		const OpenGLFrameInfo& frameInfo = *reinterpret_cast<const OpenGLFrameInfo*>(frameInfoStruct);

		for (const DrawInfo& draw : mDrawList->GetList()) {
			if (draw.mData.mFlags & GHUD_DRAW_DATA_FLAG_HAS_TEXTURE) {
				const GLuint texture = *reinterpret_cast<const GLuint*>(&draw.mTextureID);
				glBindTexture(GL_RGBA8, texture);
			}
			
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
		}
	}

	ResourceObject OpenGL4Context::CreateTexture(const GLuint texture) {
		ResourceObject res{};
		res._Set((void*)texture);
		return res;
	}
	void OpenGL4Context::CreateResources() {
		
	}
}