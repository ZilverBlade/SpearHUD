#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/draw/draw_data.h>

namespace GHUD {
	class DrawList;
	struct GlobalContextInfo {
		fvec2 m_ScreenSize{};
		fvec2 m_CursorPosition{};
		float m_DisplayGamma = 2.2f;
		float m_AspectRatio = 1.0f;
	};
	class Context : public virtual NonCopyableClass {
	public:
		Context();
		virtual ~Context();
		inline DrawList* GetDrawList() { return m_DrawList; }
		virtual void Render(void* frameInfoStruct);
		virtual void Pick(fvec2 cursorCoord);
		inline const GlobalContextInfo& GetGlobalContextInfo() { return m_CtxInfo; }
		virtual void Resize(fvec2 screenSize);
	protected:
		DrawList* m_DrawList{};
		GlobalContextInfo m_CtxInfo{};
	};
}
