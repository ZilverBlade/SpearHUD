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
		Context();
		virtual ~Context();
		inline DrawList* GetDrawList() { return mDrawList; }
		virtual void SetGamma(float gamma);
		virtual void Render(const void* frameInfoStruct);
		virtual void Pick(fvec2 cursorCoord);
		inline const GlobalContextInfo& GetGlobalContextInfo() { return mCtxInfo; }
		virtual void Resize(fvec2 screenSize);
	protected:
		DrawList* mDrawList{};
		GlobalContextInfo mCtxInfo{};
	};
}
