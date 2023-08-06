#include <shudcpp/context.h>
#include <shudcpp/draw/draw_list.h>
#include <shudcpp/resources/text_object.h>

namespace SHUD {
	Context::Context() {
		mDrawList = new DrawList(this);
	}

	Context::~Context() {
		delete mDrawList;
	}

	void Context::SetGamma(float gamma) {
		mCtxData.mDisplayGamma = gamma;
		mCtxData.mInvDisplayGamma = 1.0 / gamma;
	}
	void Context::UpdateIOFrame() {
		mIOLastFrame = mIO;
	}

	BufferID Context::AllocateTextBuffer(size_t id, const std::string& characters, const TextFormatting& formatting) {
		assert(mVtblAssert != nullptr && "Text allocation function is not implemented!");
		return 0;
	}
	void Context::DeallocateTextBuffer(size_t id) {
		assert(mVtblAssert != nullptr && "Text deallocation function is not implemented!");
	}
	IO& Context::GetIO() {
		return mIO;
	}
	SHUD_API const IO& Context::GetIOLastFrame() {
		return mIOLastFrame;
	}
	SHUD_API const TextureID Context::GetFontAtlasTextureID() {
		return mDefaultFontAtlas;
	}
	void Context::Pick() {
	}
	void Context::SetResolution(fvec2 screenSize) {
		mCtxData.mResolution = screenSize;
		mCtxData.mInvResolution = fvec2(1.0f) / screenSize;
		mCtxData.mAspectRatio = screenSize.x / screenSize.y;
		mCtxData.mInvAspectRatio = screenSize.y / screenSize.x;
	}
	void Context::Render(const void* frameInfoStruct) {
		assert(mVtblAssert != nullptr && "Render function is not implemented!");
	}
}