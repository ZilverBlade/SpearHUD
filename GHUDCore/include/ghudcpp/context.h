#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/draw/draw_data.h>

namespace GHUD {
	class DrawList;
	struct GlobalContextInfo {
		fvec2 mCursorPosition{};
		fvec2 mScreenSize{};
		float mAspectRatio = 1.0f;
		float mInverseAspectRatio = 1.0f;
		float mDisplayGamma = 2.2f;
		float mDisplayInverseGamma = 1.0f / 2.2f;
	};
	class Context : public virtual NonCopyableClass {
	public:
		GHUD_API Context();
		GHUD_API virtual ~Context();
		GHUD_API virtual void SetGamma(float gamma);
		GHUD_API virtual void Render(const void* frameInfoStruct);
		GHUD_API virtual void Pick(fvec2 cursorCoord);
		GHUD_API virtual void Resize(fvec2 screenSize);

		inline DrawList* GetDrawList() { return mDrawList; }
		inline const GlobalContextInfo& GetGlobalContextInfo() { return mCtxInfo; }
	protected:
		DrawList* mDrawList{};
		GlobalContextInfo mCtxInfo{};
	};
}
