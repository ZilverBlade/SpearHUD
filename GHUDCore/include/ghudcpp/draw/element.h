#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/draw/state.h>
#include <ghudcpp/draw/draw_data.h>
#include <ghudcpp/resources/texture_object.h>
#include <ghudcpp/resources/font_object.h>

namespace GHUD {
	class DrawList;
	struct GlobalContextInfo;

	namespace Element {
		enum class Type {
			None,
			UserDefined,
			Line, // Line is a lightweight element with 2 points and a colour
			Rect, // Rect is a lightweight element with a transform (position and scale) and a color
			Image, // Image is a lightweight element with a transform (position and scale) texture and color
			Button, // Button is an interactive Image element.
			Text, // Text is an element to display text with a color.
			TextButton, // TextButton is an interactive Text element.
			TextInput, // TextInput is an interactive box to input text.
			Window, // Window is a movable (may be configured to drag) rect that may contain other elements.
			Panel, 
			Scrollbar,
			Rotor
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
			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const = 0;
		protected:
			DrawableBase() = default;
		};
		struct NonDrawableBase : public virtual Base {
		protected:
			NonDrawableBase() = default;
		};

		struct InteractiveDrawableBase : public virtual DrawableBase {
			uint32 mTabIndex = 0;
			inline PressState GetPressState() const { return mState.mPressState; }
		protected:
			State mState;
			friend class DrawList;
		};

		struct Line : public DrawableBase {
			Line() : Base(Type::Line) {}
			Line(const Line& other) : mPointA(other.mPointA), mPointB(other.mPointB), mColor(other.mColor) {}
			Line& operator=(const Line& other) { return Line(other); }

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			fvec2 mPointA = { 0.0f, 0.0f };
			fvec2 mPointB = { 1.0f, 1.0f };
			float mWidth = 1.0f;
			RGBAColor mColor{};
		};

		struct Rect : public DrawableBase {
			Rect() : Base(Type::Rect) {}
			Rect(const Rect& other) : mTransform(other.mTransform), mColor(other.mColor) {}
			Rect& operator=(const Rect& other) { return Rect(other); }

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform mTransform{};
			RGBAColor mColor{};
		};

		struct Image : public DrawableBase {
			Image() : Base(Type::Image) {}
			Image(const Image& other) : mTransform(other.mTransform), mTexture(other.mTexture), mColor(other.mColor) {}
			Image& operator=(const Image& other) { return Image(other); }

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform mTransform{};
			TextureObject mTexture{};
			fvec2 mGlobalUVOffsetMin = { 0.f, 0.f };
			fvec2 mGlobalUVOffsetMax = { 1.f, 1.f };
			RGBAColor mColor{};
		};

		struct Button : public InteractiveDrawableBase {
			Button() : Base(Type::Button) {}

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform mTransform{};
			TextureObject mTexture{};
			RGBAColor mColor{};
		};

		struct Rotor : public InteractiveDrawableBase {
			Rotor() : Base(Type::Rotor) {}

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform mTransform{};
			float mRotation{};
			TextureObject mTexture{};
			RGBAColor mColor{};
		};

		struct Text : public DrawableBase {
			Text() : Base(Type::Text) {}

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform mTransform{};
			FontObject mFont{};
			RGBAColor mColor{};
			char* mText{};
		};
		struct TextButton : public InteractiveDrawableBase {
			TextButton() : Base(Type::TextButton) {}

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform mTransform{};
			FontObject mFont{};
			RGBAColor mColor{};
			char* mText{};
		};

		struct Panel : public NonDrawableBase {
			Panel() : Base(Type::Panel) {}
			Transform mTransform{};
		};

		struct Window : public InteractiveDrawableBase {
			Window() : Base(Type::TextButton) {}
			Transform mTransform{};

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;
		protected:
			std::vector<Element::Base*> elements{}; // this will hold a reference to our elements inside the window
		};
	}
}