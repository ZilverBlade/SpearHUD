#pragma once

#include <ghudgl4/core.h>
#include <glad/glad.h>

namespace GHUD {
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
		GHUDGL4_API OpenGL4Context(const OpenGL4ContextCreateInfo& createInfo);
		GHUDGL4_API ~OpenGL4Context();
		GHUDGL4_API virtual void Render(const OpenGLFrameInfoStruct* frameInfoStruct) override;
		GHUDGL4_API ResourceObject CreateTexture(const GLuint texture);
		GHUDGL4_API void CreateResources();
	private:
		GHUDGL4_API void CreateGraphicsProgram(const OpenGL4ContextCreateInfo& createInfo);

		GLuint m_GraphicsProgram;
		GLuint m_GlobalUBO;
		GLuint m_IDSSBO;
	};
}
