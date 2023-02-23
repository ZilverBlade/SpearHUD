#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/draw/state.h>
#include <ghudcpp/draw/draw_data.h>
#include <ghudcpp/resources/texture_object.h>
#include <ghudcpp/resources/font_object.h>

namespace GHUD {
	class DrawList;
	struct GlobalContextInfo;

	struct Transform {
		fvec2 m_TransformOffset = { 0.0f, 0.0f };
		fvec2 m_Position = { 0.5f, 0.5f };
		fvec2 m_Scale = { 1.0f, 1.0f };
		float m_Rotation = 0.0f;
	};
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
			const Element::Type m_Type = Type::None;
			Base(Element::Type type) : m_Type(type) {}
			LayerIndex m_Layer = 0;
			virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const {
				return DrawData();
			}
			fvec2 m_AnchorOffset = { 0.0f, 0.0f };
		protected:
			Base() = default;
		};

		struct Interactive : public virtual Base {
			uint32 m_TabIndex = 0;
			inline PressState GetPressState() const { return m_State.m_PressState; }
		protected:
			State m_State;
			friend class DrawList;
		};

		struct Panel : public Base {
			Panel() : Base(Type::Panel) {}
			Panel(const Panel& other) : m_Transform(other.m_Transform) {}
			Panel& operator=(const Panel& other) {
				return Panel(other);
			}

			Transform m_Transform{};
		};

		struct Line : public Base {
			Line() : Base(Type::Line) {}
			Line(const Line& other) : m_PointA(other.m_PointA), m_PointB(other.m_PointB), m_Color(other.m_Color) {}
			Line& operator=(const Line& other) { return Line(other); }

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			fvec2 m_PointA = { 0.0f, 0.0f };
			fvec2 m_PointB = { 1.0f, 1.0f };
			float m_Width = 1.0f;
			RGBAColor m_Color{};
		};

		struct Rect : public Base {
			Rect() : Base(Type::Rect) {}
			Rect(const Rect& other) : m_Transform(other.m_Transform), m_Color(other.m_Color) {}
			Rect& operator=(const Rect& other) { return Rect(other); }

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			RGBAColor m_Color{};
		};

		struct Image : public Base {
			Image() : Base(Type::Image) {}
			Image(const Image& other) : m_Transform(other.m_Transform), m_Texture(other.m_Texture), m_Color(other.m_Color) {}
			Image& operator=(const Image& other) { return Image(other); }

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			TextureObject m_Texture{};
			fvec2 m_UVMin = { 0.f, 0.f };
			fvec2 m_UVMax = { 1.f, 1.f };
			RGBAColor m_Color{};
		};

		struct Button : public Interactive {
			Button() : Base(Type::Button) {}

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform Transform{};
			TextureObject Texture{};
			RGBAColor Color{};
		};

		struct Text : public Base {
			Text() : Base(Type::Text) {}

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			FontObject m_Font{};
			RGBAColor m_Color{};
			char* m_Text{};
		};
		struct TextButton : public Interactive {
			TextButton() : Base(Type::TextButton) {}

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;

			Transform m_Transform{};
			FontObject m_Font{};
			RGBAColor m_Color{};
			char* m_Text{};
		};

		struct Window : public Interactive {
			Window() : Base(Type::TextButton) {}
			Transform m_Transform{};

			GHUD_API virtual const DrawData GenerateDrawData(const GlobalContextInfo* ctxInfo) const override;
		protected:
			std::vector<Element::Base*> elements{}; // this will hold a reference to our elements inside the window
		};
	}
}