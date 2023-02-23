#include <ghudcpp/context.h>
#include <ghudcpp/draw/draw_list.h>

namespace GHUD {
	Context::Context() {
		m_DrawList = new DrawList(this);
	}

	Context::~Context() {
		delete m_DrawList;
	}

	void Context::SetGamma(float gamma) {
		m_CtxInfo.m_DisplayGamma = gamma;
		m_CtxInfo.m_DisplayInverseGamma = 1.0 / gamma;
	}
	void Context::Pick(fvec2 cursorCoord) {
		m_CtxInfo.m_CursorPosition = cursorCoord;
	}
	void Context::Resize(fvec2 screenSize) {
		m_CtxInfo.m_ScreenSize = screenSize;
		m_CtxInfo.m_AspectRatio = screenSize.x / screenSize.y;
		m_CtxInfo.m_InverseAspectRatio = screenSize.y / screenSize.x;
	}
	void Context::Render(const void* frameInfoStruct) {

	}
}