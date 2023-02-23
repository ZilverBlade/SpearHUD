#pragma once

#include <ghudcpp/context.h>
#include <ghudcpp/draw/element.h>
#include <ghudcpp/draw/draw_vector.h>
#include <set>
#include <vector>

namespace GHUD {
	class Context; 

	class DrawList : virtual public NonCopyableClass {
	public:
		GHUD_API DrawList(Context* ctx);
		GHUD_API ~DrawList();
		GHUD_API void FrameStart();
		GHUD_API void FrameEnd();
		GHUD_API void Clear();
		inline const auto& GetList() { 
			return m_DrawList;
		}
		GHUD_API const Element::Line DrawLine(const Element::Line& line);
		GHUD_API const Element::Line DrawLine(fvec2 m_PointA, fvec2 m_PointB, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset = { 0.0, 0.0 });
		GHUD_API const Element::Rect DrawRect(const Element::Rect& rect);
		GHUD_API const Element::Rect DrawRect(const Transform& m_Transform, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset = { 0.0, 0.0 });
		GHUD_API const Element::Image DrawImage(const Element::Image& img);
		GHUD_API const Element::Image DrawImage(const Transform& m_Transform, const TextureObject& m_Texture, RGBAColor m_Color, LayerIndex m_Layer, fvec2 m_AnchorOffset = { 0.0, 0.0 }, fvec2 m_UVMin = { 0.f, 0.f }, fvec2 m_UVMax = { 1.f, 1.f });
	
		GHUD_API const Element::Panel BeginPanel(const Transform& m_Transform, LayerIndex m_Layer, fvec2 m_AnchorOffset = { 0.0, 0.0 });
		GHUD_API const void EndPanel(const Element::Panel& panel);

	private:
		Context* ctx;

		std::multiset<DrawInfo> m_DrawList;

		Transform m_TransformOffset;
		LayerIndex m_LayerOffset;
		fvec2 m_AnchorOffsetOffset;

		size_t m_PreviousSize = 0;
	};
}