#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/draw/state.h>
#include <ghudcpp/draw/draw_data.h>
#include <ghudcpp/resources/texture_object.h>
#include <ghudcpp/resources/font_object.h>

namespace GHUD {
	class DrawList;
	class GlobalContextInfo;

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
			Scrollbar,
			Rotor
		};

		struct Transform {
			fvec2 m_Coord = { 0.5f, 0.5f };
			fvec2 m_Scale = { 1.0f, 1.0f };
		};

		struct Base {
			const Element::Type m_Type = Type::None;
			uint64 m_UserDefinedID = 0;
			Base(Element::Type type) : m_Type(type) {}
			LayerIndex m_Layer = 0;
			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const = 0;
			fvec2 m_AnchorOffset = { 0.0f, 0.0f };
		protected:
			Base() = default;
		};

		struct Interactive : public virtual Base {
			uint64 m_TabIndex = 0;
			inline PressState GetPressState() const { return m_State.m_PressState; }
		protected:
			State m_State;
			friend class DrawList;
		};

		struct Line : public Base {
			Line() : Base(Type::Line) {}
			Line(const Line& other) : m_PointA(other.m_PointA), m_PointB(other.m_PointB), m_Color(other.m_Color) {}
			Line& operator=(const Line& other) { return Line(other); }

			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			fvec2 m_PointA = { 0.0f, 0.0f };
			fvec2 m_PointB = { 1.0f, 1.0f };
			float m_Width = 1.0f;
			RGBAColor m_Color{};
		};

		struct Rect : public Base {
			Rect() : Base(Type::Rect) {}
			Rect(const Rect& other) : m_Transform(other.m_Transform), m_Color(other.m_Color) {}
			Rect& operator=(const Rect& other) { return Rect(other); }

			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			RGBAColor m_Color{};
		};

		struct Image : public Base {
			Image() : Base(Type::Image) {}
			Image(const Image& other) : m_Transform(other.m_Transform), m_Texture(other.m_Texture), m_Color(other.m_Color) {}
			Image& operator=(const Image& other) { return Image(other); }

			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			TextureObject m_Texture{};
			RGBAColor m_Color{};
		};

		struct Button : public Interactive {
			Button() : Base(Type::Button) {}

			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			TextureObject m_Texture{};
			RGBAColor m_Color{};
		};

		struct Rotor : public Interactive {
			Rotor() : Base(Type::Rotor) {}

			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			float m_Rotation;
			TextureObject m_Texture;
			RGBAColor m_Color;
		};

		struct Text : public Base {
			Text() : Base(Type::Text) {}

			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			FontObject m_Font{};
			RGBAColor m_Color{};
			char* m_Text;
		};
		struct TextButton : public Interactive {
			TextButton() : Base(Type::TextButton) {}

			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			FontObject m_Font{};
			RGBAColor m_Color{};
			char* m_Text;
		};

		struct Window : public Interactive {
			Window() : Base(Type::TextButton) {}
			Transform m_Transform{};

			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;
		protected:
			std::vector<Element::Base*> elements; // this will hold a reference to our elements inside the window
		};
	}
}