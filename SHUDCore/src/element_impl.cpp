#include <shudcpp/draw/element.h>
#include <shudcpp/context.h>
#include <shudcpp/utils/helper.h>

namespace SHUD {
	namespace Element {
		const DrawData Line::GenerateDrawData(const GlobalContextData* ctxData) const {
			DrawData data{};

			fvec2 ptA = Utils::ConvertScreenCoordToGPUCoord(mPointA, ctxData->mInvResolution);
			fvec2 ptB = Utils::ConvertScreenCoordToGPUCoord(mPointB, ctxData->mInvResolution);

			ptA.x *= ctxData->mAspectRatio;
			ptB.x *= ctxData->mAspectRatio;

			fvec2 v = ptB - ptA;
			float len = Math::Length(v);
			fvec2 u = fvec2(v.x, v.y) / len;

			const float c = u.x;
			const float s = u.y;

			const float w = mWidth * ctxData->mInvResolution.y ;
			data.mAnchorOffset = mAnchorOffset;
			
			data.mRotationMatrix[0][0] = v.x * 0.5f * ctxData->mInvAspectRatio;
			data.mRotationMatrix[0][1] = v.y * 0.5f ;
			data.mRotationMatrix[1][0] = -u.y * w * ctxData->mInvAspectRatio;
			data.mRotationMatrix[1][1] = u.x * w;
			data.mPosition = (ptA + ptB) * 0.5f;
			data.mPosition.x *= ctxData->mInvAspectRatio;
			data.mColorCompressed = Utils::RGBAColorToUnsigned32(mColor);

			return data;
		}
		// 0 - 1
		const DrawData Rect::GenerateDrawData(const GlobalContextData* ctxData) const {
			DrawData data{};
			data.mAnchorOffset = mAnchorOffset;
			data.mTransformOffset = mTransform.mTransformOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition, ctxData->mInvResolution);
			fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mTransform.mScale, ctxData->mInvResolution);
			if (fmodf(abs(mTransform.mRotation), 6.28318530718f) > std::numeric_limits<float>::epsilon()) {
				const float s = sin(mTransform.mRotation);
				const float c = cos(mTransform.mRotation);
				data.mRotationMatrix[0][0] = c * uvScale.x;
				data.mRotationMatrix[0][1] = s * uvScale.x * ctxData->mAspectRatio;
				data.mRotationMatrix[1][0] = -s * uvScale.y * ctxData->mInvAspectRatio;
				data.mRotationMatrix[1][1] = c * uvScale.y;
			} else {
				data.mRotationMatrix[0][0] = uvScale.x;
				data.mRotationMatrix[1][1] = uvScale.y;
			}

			data.mColorCompressed = Utils::RGBAColorToUnsigned32(mColor);
			return data;
		}

		const DrawData SHUD::Element::Image::GenerateDrawData(const GlobalContextData* ctxData) const {
			DrawData data{};

			data.mAnchorOffset = mAnchorOffset;
			data.mTransformOffset = mTransform.mTransformOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition, ctxData->mInvResolution);
			fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mTransform.mScale, ctxData->mInvResolution);

			if (fmodf(abs(mTransform.mRotation), 6.28318530718f) > std::numeric_limits<float>::epsilon()) {
				const float s = sin(mTransform.mRotation);
				const float c = cos(mTransform.mRotation);
				data.mRotationMatrix[0][0] = c * uvScale.x;
				data.mRotationMatrix[0][1] = s * uvScale.x * ctxData->mAspectRatio;
				data.mRotationMatrix[1][0] = -s * uvScale.y * ctxData->mInvAspectRatio;
				data.mRotationMatrix[1][1] = c * uvScale.y;
			} else {
				data.mRotationMatrix[0][0] = uvScale.x;
				data.mRotationMatrix[1][1] = uvScale.y;
			}

			data.mFlags |= SHUD_DRAW_DATA_FLAG_HAS_TEXTURE;
			data.mUVOffsetA = mGlobalUVOffsetMin;
			data.mUVOffsetB = mGlobalUVOffsetMax;
			data.mSubUVOffsetA = mTexture.mDefaultTextureCoords.mUVOffsetMin;
			data.mSubUVOffsetB = mTexture.mDefaultTextureCoords.mUVOffsetMax;

			data.mColorCompressed = Utils::RGBAColorToUnsigned32(mColor);
			return data;
		}
		const DrawData SHUD::Element::Button::GenerateDrawData(const GlobalContextData* ctxData) const {
			DrawData data{};

			data.mAnchorOffset = mAnchorOffset;
			data.mTransformOffset = mTransform.mTransformOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition, ctxData->mInvResolution);
			fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mTransform.mScale, ctxData->mInvResolution);
			if (fmodf(abs(mTransform.mRotation), 6.28318530718f) > std::numeric_limits<float>::epsilon()) {
				const float s = sin(mTransform.mRotation);
				const float c = cos(mTransform.mRotation);
				data.mRotationMatrix[0][0] = c * uvScale.x;
				data.mRotationMatrix[0][1] = s * uvScale.x * ctxData->mAspectRatio;
				data.mRotationMatrix[1][0] = -s * uvScale.y * ctxData->mInvAspectRatio;
				data.mRotationMatrix[1][1] = c * uvScale.y;
			} else {
				data.mRotationMatrix[0][0] = uvScale.x;
				data.mRotationMatrix[1][1] = uvScale.y;
			}

			data.mFlags |= SHUD_DRAW_DATA_FLAG_HAS_TEXTURE;
			data.mFlags |= SHUD_DRAW_DATA_FLAG_HAS_INTERACTION;
			data.mUVOffsetA = { 0.0f, 0.0f };
			data.mUVOffsetB = { 1.0f, 1.0f };
			data.mSubUVOffsetA = mTexture.mDefaultTextureCoords.mUVOffsetMin;
			data.mSubUVOffsetB = mTexture.mDefaultTextureCoords.mUVOffsetMax;

			data.mColorCompressed = Utils::RGBAColorToUnsigned32(mColor);
			return data;
		}
		const DrawData SHUD::Element::Text::GenerateDrawData(const GlobalContextData* ctxData) const {

			DrawData data{};

			data.mAnchorOffset = mAnchorOffset;
			data.mTransformOffset = mTransform.mTransformOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition, ctxData->mInvResolution);
			fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mTransform.mScale, ctxData->mInvResolution);
			if (fmodf(mTransform.mRotation * mTransform.mRotation, 6.28318530718f * 6.28318530718f) > 1e-10f) {
				const float s = sin(mTransform.mRotation);
				const float c = cos(mTransform.mRotation);
				data.mRotationMatrix[0][0] = c * uvScale.x;
				data.mRotationMatrix[0][1] = s * uvScale.x * ctxData->mAspectRatio;
				data.mRotationMatrix[1][0] = -s * uvScale.y * ctxData->mInvAspectRatio;
				data.mRotationMatrix[1][1] = c * uvScale.y;
			} else {
				data.mRotationMatrix[0][0] = uvScale.x;
				data.mRotationMatrix[1][1] = uvScale.y;
			}

			data.mFlags |= SHUD_DRAW_DATA_FLAG_HAS_TEXTURE | SHUD_DRAW_DATA_FLAG_IS_MSDF_TEXT;
			data.mUVOffsetA = { 0.f, 0.f };
			data.mUVOffsetB = { 1.f, 1.f };
			data.mSubUVOffsetA = { 0.f, 0.f };
			data.mSubUVOffsetB = { 1.f, 1.f };
			data.mColorCompressed = *reinterpret_cast<const uint32*>(&mColor);

			return data;
		}
		const DrawData SHUD::Element::TextButton::GenerateDrawData(const GlobalContextData* ctxData) const {
			assert(false && "not implemented");
			return DrawData();
		}
		const DrawData SHUD::Element::Window::GenerateDrawData(const GlobalContextData* ctxData) const {
			assert(false && "not implemented");
			return DrawData();
		}
	}
}