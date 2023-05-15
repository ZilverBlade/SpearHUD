#include <ghudcpp/draw/element.h>
#include <ghudcpp/context.h>
#include <ghudcpp/utils/helper.h>

namespace GHUD {
	namespace Element {

		const DrawData Line::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			DrawData data{};

			fvec2 ptA = Utils::ConvertScreenCoordToGPUCoord(mPointA, ctxInfo->mInvResolution);
			fvec2 ptB = Utils::ConvertScreenCoordToGPUCoord(mPointB, ctxInfo->mInvResolution);

			ptA.x *= ctxInfo->mAspectRatio;
			ptB.x *= ctxInfo->mAspectRatio;

			fvec2 v = ptB - ptA;
			float len = Math::Length(v);
			fvec2 u = fvec2(v.x, v.y) / len;

			const float c = u.x;
			const float s = u.y;

			const float w = mWidth * ctxInfo->mInvResolution.y ;
			data.mAnchorOffset = mAnchorOffset;
			
			data.mRotationMatrix[0][0] = v.x * 0.5f * ctxInfo->mInvAspectRatio;
			data.mRotationMatrix[0][1] = v.y * 0.5f ;
			data.mRotationMatrix[1][0] = -u.y * w * ctxInfo->mInvAspectRatio;
			data.mRotationMatrix[1][1] = u.x * w;
			data.mPosition = (ptA + ptB) * 0.5f;
			data.mPosition.x *= ctxInfo->mInvAspectRatio;
			data.mColorCompressed = *reinterpret_cast<const uint32*>(&mColor);
			return data;
		}
		// 0 - 1
		const DrawData Rect::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			DrawData data{};
			data.mAnchorOffset = mAnchorOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition, ctxInfo->mInvResolution);
			fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mTransform.mScale, ctxInfo->mInvResolution);
			data.mRotationMatrix[0][0] = uvScale.x;
			data.mRotationMatrix[1][1] = uvScale.y;

			data.mColorCompressed = *reinterpret_cast<const uint32*>(&mColor);
			return data;
		}

		const DrawData Rotor::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			DrawData data{};

			const float s = sin(mRotation);
			const float c = cos(mRotation);

			data.mAnchorOffset = mAnchorOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition, ctxInfo->mInvResolution);
			fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mTransform.mScale, ctxInfo->mInvResolution);
			data.mRotationMatrix[0][0] = c * uvScale.x;
			data.mRotationMatrix[0][1] = s * uvScale.x;
			data.mRotationMatrix[1][0] = -s * uvScale.y;
			data.mRotationMatrix[1][1] = c * uvScale.y;

			data.mColorCompressed = *reinterpret_cast<const uint32*>(&mColor);
			return data;
		}
		const DrawData GHUD::Element::Image::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			DrawData data{};

			data.mAnchorOffset = mAnchorOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition, ctxInfo->mInvResolution);
			fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mTransform.mScale, ctxInfo->mInvResolution);
			data.mRotationMatrix[0][0] = uvScale.x;
			data.mRotationMatrix[1][1] = uvScale.y;

			data.mFlags |= GHUD_DRAW_DATA_FLAG_HAS_TEXTURE;
			data.mUVOffsetA = mGlobalUVOffsetMin;
			data.mUVOffsetB = mGlobalUVOffsetMax;
			data.mSubUVOffsetA = mTexture.mDefaultTextureCoords.mUVOffsetMin;
			data.mSubUVOffsetB = mTexture.mDefaultTextureCoords.mUVOffsetMax;

			data.mColorCompressed = *reinterpret_cast<const uint32*>(&mColor);
			return data;
		}
		const DrawData GHUD::Element::Button::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			DrawData data{};

			data.mAnchorOffset = mAnchorOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition, ctxInfo->mInvResolution);
			fvec2 uvScale = Utils::ConvertPixelScaleToUVScale(mTransform.mScale, ctxInfo->mInvResolution);
			data.mRotationMatrix[0][0] = uvScale.x;
			data.mRotationMatrix[1][1] = uvScale.y;

			data.mFlags |= GHUD_DRAW_DATA_FLAG_HAS_TEXTURE;
			data.mFlags |= GHUD_DRAW_DATA_FLAG_HAS_INTERACTION;
			data.mUVOffsetA = { 0.0f, 0.0f };
			data.mUVOffsetB = { 1.0f, 1.0f };
			data.mSubUVOffsetA = mTexture.mDefaultTextureCoords.mUVOffsetMin;
			data.mSubUVOffsetB = mTexture.mDefaultTextureCoords.mUVOffsetMax;

			data.mColorCompressed = *reinterpret_cast<const uint32*>(&mColor);
			return data;
		}
		const DrawData GHUD::Element::Text::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			assert(false && "not implemented");
			return DrawData();
		}
		const DrawData GHUD::Element::TextButton::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			assert(false && "not implemented");
			return DrawData();
		}
		const DrawData GHUD::Element::Window::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			assert(false && "not implemented");
			return DrawData();
		}
	}
}