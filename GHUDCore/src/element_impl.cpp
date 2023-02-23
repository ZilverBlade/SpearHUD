#include <ghudcpp/draw/element.h>
#include <ghudcpp/context.h>
#include <ghudcpp/utils/helper.h>

namespace GHUD {
	namespace Element {
		const DrawData Line::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			DrawData data{};

			fvec2 ptA = Utils::ConvertScreenCoordToGPUCoord(mPointA);
			fvec2 ptB = Utils::ConvertScreenCoordToGPUCoord(mPointB);
			fvec2 v = ptB - ptA;
			float len = Math::Length(v);
			fvec2 u = fvec2(v.x, v.y) / len;

			const float c = u.x;
			const float s = u.y;

			const float w = mWidth / ctxInfo->mScreenSize.y;

			data.mAnchorOffset = mAnchorOffset;
			data.mRotationMatrix[0][0] = v.x * 0.5;
			data.mRotationMatrix[0][1] = v.y * 0.5;
			data.mRotationMatrix[1][0] = -u.y * w;
			data.mRotationMatrix[1][1] = u.x * w;
			data.mPosition = (ptA + ptB) * 0.5f;

			data.mColor = Utils::ConvertHexColorToFloat4Color(mColor);
			return data;
		}
		// 0 - 1
		const DrawData Rect::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			DrawData data{};
			data.mAnchorOffset = mAnchorOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition);

			data.mRotationMatrix[0][0] = mTransform.mScale.x;
			data.mRotationMatrix[1][1] = mTransform.mScale.y;

			data.mColor = Utils::ConvertHexColorToFloat4Color(mColor);
			return data;
		}

		const DrawData Rotor::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			DrawData data{};

			const float s = sin(mRotation);
			const float c = cos(mRotation);

			data.mAnchorOffset = mAnchorOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition);
			data.mRotationMatrix[0][0] = c * mTransform.mScale.x;
			data.mRotationMatrix[0][1] = s * mTransform.mScale.x;
			data.mRotationMatrix[1][0] = -s * mTransform.mScale.y;
			data.mRotationMatrix[1][1] = c * mTransform.mScale.y;

			data.mColor = Utils::ConvertHexColorToFloat4Color(mColor);
			return data;
		}
		const DrawData GHUD::Element::Image::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			DrawData data{};

			data.mAnchorOffset = mAnchorOffset;
			data.mPosition = Utils::ConvertScreenCoordToGPUCoord(mTransform.mPosition);
			data.mRotationMatrix[0][0] = mTransform.mScale.x;
			data.mRotationMatrix[1][1] = mTransform.mScale.y;

			data.mHasTexture = 1;
			data.mHasInteraction = 0;
			data.mColor = Utils::ConvertHexColorToFloat4Color(mColor);
			data.mUVOffsetA = mGlobalUVOffsetMin;
			data.mUVOffsetB = mGlobalUVOffsetMax;
			data.mSubUVOffsetA = mTexture.mDefaultTextureCoords.mUVOffsetMin;
			data.mSubUVOffsetB = mTexture.mDefaultTextureCoords.mUVOffsetMax;

			return data;
		}
		const DrawData GHUD::Element::Button::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			return DrawData();
		}
		const DrawData GHUD::Element::Text::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			return DrawData();
		}
		const DrawData GHUD::Element::TextButton::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			return DrawData();
		}
		const DrawData GHUD::Element::Window::GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
			return DrawData();
		}
	}
}