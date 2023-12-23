#pragma once

#include <shudcpp/core.h>
#include <shudcpp/draw/state.h>
#include <shudcpp/draw/draw_data.h>
#include <shudcpp/resources/texture_object.h>
#include <shudcpp/resources/text_object.h>
#include <xhash>

namespace SHUD {
	class DrawList;
	struct GlobalContextData;

	namespace Element {
		enum class Type : uint32 {
			None = 0,
			UserDefined = 1,
			Line = 2, // Line is a lightweight element with 2 points and a colour
			Rect = 3, // Rect is a lightweight element with a transform (position and scale) and a color
			Image = 4, // Image is a lightweight element with a transform (position and scale) texture and color
			Button = 5, // Button is an interactive Image element.
			Text = 6, // Text is an element to display text with a color.
			TextButton = 7, // -- NOT IMPLEMENTED -- TextButton is an interactive Text element.
			TextInput = 8, // -- NOT IMPLEMENTED -- TextInput is an interactive box to input text.
			Window = 9, // -- NOT IMPLEMENTED -- Window is a movable (may be configured to drag) rect that may contain other elements.
			Panel = 10, // Panel is a way to group elements together, has it's own transform, anchor offset, and layer
			Grid = 11, // -- NOT IMPLEMENTED -- Grid is a way to efficiently tile elements in a grid like pattern.
			Scrollbar = 12 // -- NOT IMPLEMENTED --
		};

		struct Base {
			const Element::Type mType = Type::None;
			Base(Element::Type type) : mType(type) {}
			LayerIndex mLayer = 0;
			fvec2 mAnchorOffset = { 0.0f, 0.0f };
		protected:
			Base() = default;
		};
		struct DrawableBase : public virtual Base {
			virtual const DrawData GenerateDrawData(const GlobalContextData* ctxData) const = 0;
		protected:
			DrawableBase() = default;
		};
		struct NonDrawableBase : public virtual Base {
		protected:
			NonDrawableBase() = default;
		};

		struct InteractiveDrawableBase : public virtual DrawableBase {
			uint32 mTabIndex = 0;
			size_t mElementID = 0x00;
			PressState GetPressState() const { return PressState(mPressState); }
			void SetUniqueTag(const std::string& string) {
				static std::hash<std::string> hashFunc{};
				mElementID = hashFunc(string);
			}
		protected:
			PressStateFlags mPressState = SHUD_PRESS_STATE_FLAG_NONE;
			friend class DrawList;
		};

		struct Line : public DrawableBase {
			Line() : Base(Type::Line) {}
			Line(const Line& other) : mPointA(other.mPointA), mPointB(other.mPointB), mColor(other.mColor) {}
			Line& operator=(const Line& other) { return Line(other); }

			SHUD_API virtual const DrawData GenerateDrawData(const GlobalContextData* ctxData) const override;

			fvec2 mPointA = { 0.0f, 0.0f };
			fvec2 mPointB = { 1.0f, 1.0f };
			float mWidth = 1.0f;
			RGBAColor mColor = 0xFFFFFFFF;
		};

		struct Rect : public DrawableBase {
			Rect() : Base(Type::Rect) {}
			Rect(const Rect& other) : mTransform(other.mTransform), mColor(other.mColor) {}
			Rect& operator=(const Rect& other) { return Rect(other); }

			SHUD_API virtual const DrawData GenerateDrawData(const GlobalContextData* ctxData) const override;

			Transform mTransform{};
			RGBAColor mColor = 0xFFFFFFFF;
		};

		struct Image : public DrawableBase {
			Image() : Base(Type::Image) {}
			Image(const Image& other) : mTransform(other.mTransform), mTexture(other.mTexture), mColor(other.mColor) {}
			Image& operator=(const Image& other) { return Image(other); }

			SHUD_API virtual const DrawData GenerateDrawData(const GlobalContextData* ctxData) const override;

			Transform mTransform{};
			TextureObject mTexture{};
			fvec2 mGlobalUVOffsetMin = { 0.f, 0.f };
			fvec2 mGlobalUVOffsetMax = { 1.f, 1.f };
			RGBAColor mColor = 0xFFFFFFFF;
		};

		struct Button : public InteractiveDrawableBase {
			Button() : Base(Type::Button) {}

			SHUD_API virtual const DrawData GenerateDrawData(const GlobalContextData* ctxData) const override;

			Transform mTransform{};
			TextureObject mTexture{};
			float mAlphaButtonCutoff = 0.5f;
			RGBAColor mColor = 0xFFFFFFFF;
		};

		struct Text : public DrawableBase {
			Text() : Base(Type::Text) {
				mTransform.mScale = { 1.f, 1.f }; // scale is multiplied by font size, reset to 1;
			}

			SHUD_API virtual const DrawData GenerateDrawData(const GlobalContextData* ctxData) const override;

			void FormatText(const FormattedText& t) {
				mText = t.mText;
			}
			TextFormatting mFormatting{};
			Transform mTransform{};
			std::string mText;
			RGBAColor mColor = 0xFFFFFFFF;
		};
		struct TextButton : public InteractiveDrawableBase {
			TextButton() : Base(Type::TextButton) {}

			SHUD_API virtual const DrawData GenerateDrawData(const GlobalContextData* ctxData) const override;

			Transform mTransform{};
			std::string mText;
			RGBAColor mColor = 0xFFFFFFFF;
		};

		struct Panel : public NonDrawableBase {
			Panel() : Base(Type::Panel) {}
			Transform mTransform{};
		};

		struct Grid : public NonDrawableBase {
			Grid() : Base(Type::Grid) {}
			fvec2 mPosition = { 0.5f, 0.5f };
			fvec2 mMaxGridArea = { 0.5f, 0.25f };
			uint32 mElementSizePX = 64u;
			bool mTryCenterElements = false;
		};


		struct Window : public InteractiveDrawableBase {
			Window() : Base(Type::TextButton) {}
			Transform mTransform{};

			SHUD_API virtual const DrawData GenerateDrawData(const GlobalContextData* ctxData) const override;
		protected:
			std::vector<Element::Base*> elements{}; // this will hold a reference to our elements inside the window
		};
	}
}