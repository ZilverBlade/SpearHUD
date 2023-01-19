#pragma once

#include <ghudcpp/context.h>
#include <ghudcpp/draw/element.h>
#include <ghudcpp/draw/draw_vector.h>
#include <set>

namespace GHUD {
	class Context; // fwd declare

	class DrawList : virtual public NonCopyableClass {
	public:
		DrawList(Context* ctx);
		~DrawList();
		void FrameStart();
		void FrameEnd();
		void Clear();
		inline const auto& GetList() { 
			return m_DrawList;
		}
		const Element::Line DrawLine(const Element::Line& line);
		const Element::Line DrawLine(fvec2 m_PointA, fvec2 m_PointB, RGBAColor m_Color, LayerIndex m_Layer);
		const Element::Rect DrawRect(const Element::Rect& rect);
		const Element::Rect DrawRect(const Element::Transform& m_Transform, const TextureObject& m_Texture, RGBAColor m_Color, LayerIndex m_Layer);
	private:
		Context* ctx;

		ElementVector<DrawData> m_DrawData;
		std::multiset<DrawInfo> m_DrawList; // ordered version of the vector, based on the layers

		size_t m_PreviousSize = 0;
	};
}