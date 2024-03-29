#pragma once

#include <shudcpp/core.h>
#include <shudcpp/draw/draw_data.h>
#include <shudcpp/io.h>

namespace SHUD {
	class DrawList;
	inline namespace Resources {
		struct TextShaderBuffer;
	}
	struct GlobalContextData {
		fvec2 mResolution{ 1280.f, 720.f };
		fvec2 mInvResolution{ 1.f / 1280.f, 1.f / 720.f };
		float mAspectRatio = 16.f / 9.f;
		float mInvAspectRatio = 9.f / 16.f;

		float mDisplayGamma = 2.2f;
		float mInvDisplayGamma = 1.0f / 2.2f;

		uint64_t selectedObject = 0;
	};
	struct TextFormatting;

	class Context : public virtual NonCopyableClass {
	public:
		SHUD_API Context();
		SHUD_API virtual ~Context();

		SHUD_API virtual void SetGamma(float gamma);
		// Update last frame's selected object
		SHUD_API virtual void Pick();

		// Render all given elements (must be implemented for corresponding API)
		SHUD_API virtual void Render(const void* frameInfoStruct);

		// Update resolution
		SHUD_API virtual void SetResolution(fvec2 windowResolution);

		inline DrawList* GetDrawList() {
			return mDrawList;
		}

		inline const GlobalContextData& GetContextData() {
			return mCtxData;
		}
		// Prepare next IO frame
		SHUD_API virtual void UpdateIOFrame() final;
		// Get IO
		SHUD_API virtual IO& GetIO() final;
		// Get IO from last frame (compare button states e.g. ensure single time event)
		SHUD_API virtual const IO& GetIOLastFrame() final;

		SHUD_API virtual const TextureID GetFontAtlasTextureID() final;

		SHUD_API virtual BufferID AllocateTextBuffer(size_t id, const std::string& characters, const TextFormatting& formatting);
		SHUD_API virtual void DeallocateTextBuffer(size_t id);
	protected:
		DrawList* mDrawList{};
		Context* mCtx;
		GlobalContextData mCtxData{};
		IO mIO{};
		IO mIOLastFrame{};
		TextureID mDefaultFontAtlas{};
		void* mVtblAssert = nullptr;
		std::unordered_map<size_t, TextShaderBuffer*> mTextBuffers;
	};

}
