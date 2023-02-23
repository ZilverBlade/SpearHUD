#include <ghudcpp/context.h>
#include <ghudcpp/draw/draw_list.h>

namespace GHUD {
	Context::Context() {
		mDrawList = new DrawList(this);
	}

	Context::~Context() {
		delete mDrawList;
	}

	void Context::SetGamma(float gamma) {
		mCtxInfo.mDisplayGamma = gamma;
		mCtxInfo.mDisplayInverseGamma = 1.0 / gamma;
	}
	void Context::Pick(fvec2 cursorCoord) {
		mCtxInfo.mCursorPosition = cursorCoord;
	}
	void Context::Resize(fvec2 screenSize) {
		mCtxInfo.mScreenSize = screenSize;
		mCtxInfo.mAspectRatio = screenSize.x / screenSize.y;
		mCtxInfo.mInverseAspectRatio = screenSize.y / screenSize.x;
	}
	void Context::Render(const void* frameInfoStruct) {

	}
}