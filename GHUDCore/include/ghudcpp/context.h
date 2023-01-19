#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/draw/draw_data.h>

namespace GHUD {
	class DrawList;
	struct GlobalContextInfo {
		fvec2 m_CursorPosition{};
		fvec2 m_ScreenSize{};
		float m_AspectRatio = 1.0f;
		float m_DisplayGamma = 2.2f;
		float m_DisplayInverseGamma = 1.0 / 2.2f;
	};
	class Context : public virtual NonCopyableClass {
	public:
		Context();
		virtual ~Context();
		inline DrawList* GetDrawList() { return m_DrawList; }
		virtual void SetGamma(float gamma);
		virtual void Render(const void* frameInfoStruct);
		virtual void Pick(fvec2 cursorCoord);
		inline const GlobalContextInfo& GetGlobalContextInfo() { return m_CtxInfo; }
		virtual void Resize(fvec2 screenSize);
	protected:
		DrawList* m_DrawList{};
		GlobalContextInfo m_CtxInfo{};
	};
}
