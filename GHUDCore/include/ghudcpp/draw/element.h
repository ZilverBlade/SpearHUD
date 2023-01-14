#pragma once

#include <ghudcpp/core.h>
#include <ghudcpp/draw/state.h>
#include <ghudcpp/resources/texture_object.h>

namespace GHUD {
	namespace Element {
		enum class Type {
			None,
			UserDefined,
			Rect,
			Button,
			Text,
			Window,
			Scrollbar
		};

		struct ElementBase {
			const Element::Type m_Type = Type::None;
			uint64 m_UserDefinedID = 0;
			uint64 m_TabIndex = 0;
			ElementBase(Element::Type type) : m_Type(type) {}
			inline PressState GetPressState() const { return m_State.m_PressState; }
		protected:
			State m_State;
		};

		struct Rect : public ElementBase {
			Rect() : ElementBase(Type::Rect) {}
			fvec2 m_Coord = { 0.5f, 0.5f };
			fvec2 m_AnchorOffset = { 0.0f, 0.0f };
			fvec2 m_Scale = { 1.0f, 1.0f };
			ResourceObject m_Atlas;
		};
	}
}