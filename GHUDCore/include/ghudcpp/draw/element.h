#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/draw/state.h>
#include <ghudcpp/resources/texture_object.h>
#include <ghudcpp/resources/font_object.h>

namespace GHUD {
	class DrawList;
	namespace Element {
		enum class Type {
			None,
			UserDefined,
			Rect,
			Text,
			TextInput,
			Window,
			Scrollbar,
			Rotor
		};

		struct ElementBase {
			const Element::Type m_Type = Type::None;
			uint64 m_UserDefinedID = 0;
			uint64 m_TabIndex = 0;
			ElementBase(Element::Type type) : m_Type(type) {}
			inline PressState GetPressState() const {
				return m_State.m_PressState;
			}
		protected:
			State m_State;
			friend class DrawList;
		};

		struct Transform {
			fvec2 m_AnchorOffset = { 0.0f, 0.0f };
			fvec2 m_Coord = { 0.5f, 0.5f };
			fvec2 m_Scale = { 1.0f, 1.0f };
		};

		struct Rect : public ElementBase {
			Rect() : ElementBase(Type::Rect) {}
			Transform m_Transform{};
			TextureObject m_Atlas{};
			RGBAColor m_Color{};
		};

		struct Rotor : public ElementBase {
			Rotor() : ElementBase(Type::Rotor) {}
			Transform m_Transform{};
			float m_Rotation;
			ResourceObject m_Atlas;
			RGBAColor m_Color;
		};

		struct Text : public ElementBase {
			Text() : ElementBase(Type::Rect) {}
			Transform m_Transform{};
			FontObject m_Atlas{};
			RGBAColor m_Color{};
			char* m_Text;
		};

	}
}
void gaming() {
	for (;;) {
		GHUD::Element::Rotor needle{};
		needle.m_Transform.m_AnchorOffset = { 1.0f, 0.f };
		needle.m_Color = 0xFF00FFFF;
		drawlist->DrawRotor(needle);
	}
}