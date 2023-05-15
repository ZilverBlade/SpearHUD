#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/draw/draw_data.h>
#include <ghudcpp/io.h>

namespace GHUD {
	class DrawList;
	struct GlobalContextInfo {
		fvec2 mResolution{ 1280.f, 720.f };
		fvec2 mInvResolution{ 1.f / 1280.f, 1.f / 720.f };
		float mAspectRatio = 16.f / 9.f;
		float mInvAspectRatio = 9.f / 16.f;

		float mDisplayGamma = 2.2f;
		float mInvDisplayGamma = 1.0f / 2.2f;

		uint64_t selectedObject = 0;
	};
	class Context : public virtual NonCopyableClass {
	public:
		GHUD_API Context();
		GHUD_API virtual ~Context();
		GHUD_API virtual void SetGamma(float gamma);

		// Prepare next IO frame
		GHUD_API virtual void UpdateIOFrame() final;
		// Get IO
		GHUD_API virtual IO& GetIO() final;
		// Get IO from last frame (compare button states e.g. ensure single time event)
		GHUD_API virtual const IO& GetIOLastFrame() final;

		// Update last frame's selected object
		GHUD_API virtual void Pick();

		// Render all given elements (must be implemented for corresponding API)
		GHUD_API virtual void Render(const void* frameInfoStruct); 

		// Update resolution
		GHUD_API virtual void SetResolution(fvec2 windowResolution);

		inline DrawList* GetDrawList() { return mDrawList; }
		inline const GlobalContextInfo& GetGlobalContextInfo() { return mCtxInfo; }
	protected:
		DrawList* mDrawList{};
		GlobalContextInfo mCtxInfo{};
		IO mIO{};
		IO mIOLastFrame{};
		void* mVtblAssert = nullptr;
	};
}
