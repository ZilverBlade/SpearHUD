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
		mCtxInfo.mInvDisplayGamma = 1.0 / gamma;
	}
	void Context::UpdateIOFrame() {
		mIOLastFrame = mIO;
	}
	IO& Context::GetIO() {
		return mIO;
	}
	GHUD_API const IO& Context::GetIOLastFrame() {
		return mIOLastFrame;
	}
	void Context::Pick() {
	}
	void Context::SetResolution(fvec2 screenSize) {
		mCtxInfo.mResolution = screenSize;
		mCtxInfo.mInvResolution = fvec2(1.0f) / screenSize;
		mCtxInfo.mAspectRatio = screenSize.x / screenSize.y;
		mCtxInfo.mInvAspectRatio = screenSize.y / screenSize.x;
	}
	void Context::Render(const void* frameInfoStruct) {
		assert(mVtblAssert != nullptr && "Render function is not implemented!");
	}
}