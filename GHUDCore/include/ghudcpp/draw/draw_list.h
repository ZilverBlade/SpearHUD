#pragma once

#include <ghudcpp/context.h>
#include <ghudcpp/draw/element.h>
#include <ghudcpp/draw/draw_vector.h>
#include <set>
#include <vector>

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
		const Element::Line DrawLine(fvec2 m_PointA, fvec2 m_PointB, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset = { 0.0, 0.0 });
		const Element::Rect DrawRect(const Element::Rect& rect);
		const Element::Rect DrawRect(const Element::Transform& m_Transform, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset = { 0.0, 0.0 });
		const Element::Image DrawImage(const Element::Image& img);
		const Element::Image DrawImage(const Element::Transform& m_Transform, const TextureObject& m_Texture, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset = { 0.0, 0.0 }, fvec2 m_GlobalUVOffsetMin = { 0.f, 0.f }, fvec2 m_GlobalUVOffsetMax = { 1.f, 1.f });
	private:
		Context* ctx;

		std::multiset<DrawInfo> m_DrawList; // ordered version of the vector, based on the layers

		size_t m_PreviousSize = 0;
	};
}