#pragma once

#include <shudgl4/core.h>
#include <glad/glad.h>

namespace SHUD {
	struct OpenGL4ContextCreateInfo {
		char* m_VshCodeOverride = nullptr;
		char* m_FshCodeOverride = nullptr;
	};

	struct OpenGLFrameInfo {
		uint32 m_FrameIndex;
	};

	using OpenGLFrameInfoStruct = void;

	class OpenGL4Context : public Context {
	public:
		SHUDGL4_API OpenGL4Context(const OpenGL4ContextCreateInfo& createInfo);
		SHUDGL4_API ~OpenGL4Context();
		SHUDGL4_API virtual void Render(const OpenGLFrameInfoStruct* frameInfoStruct) override;
		SHUDGL4_API ResourceObject CreateTexture(const GLuint texture);
		SHUDGL4_API void CreateResources();
	private:
		SHUDGL4_API void CreateGraphicsProgram(const OpenGL4ContextCreateInfo& createInfo);

		GLuint m_GraphicsProgram;
		GLuint m_GlobalUBO;
		GLuint m_IDSSBO;
	};
}
