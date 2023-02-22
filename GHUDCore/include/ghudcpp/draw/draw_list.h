#pragma once

#include <ghudcpp/context.h>
#include <ghudcpp/draw/element.h>
#include <ghudcpp/draw/push_stack.h>
#include <ghudcpp/draw/draw_vector.h>
#include <set>
#include <vector>

namespace GHUD {
	class Context; // fwd declare

	class DrawList : virtual public NonCopyableClass {
	public:
		GHUD_API DrawList(Context* ctx);
		GHUD_API ~DrawList();
		GHUD_API void FrameStart();
		GHUD_API void FrameEnd();
		GHUD_API void Clear();
		inline const auto& GetList() { 
			return mDrawList;
		}

		// Draw functions

		GHUD_API const Element::Line DrawLine(const Element::Line& line);
		GHUD_API const Element::Line DrawLine(fvec2 mPointA, fvec2 mPointB, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset = { 0.0, 0.0 });
		GHUD_API const Element::Rect DrawRect(const Element::Rect& rect);
		GHUD_API const Element::Rect DrawRect(const Transform& mTransform, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset = { 0.0, 0.0 });
		GHUD_API const Element::Image DrawImage(const Element::Image& img);
		GHUD_API const Element::Image DrawImage(const Transform& mTransform, const TextureObject& mTexture, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset = { 0.0, 0.0 }, fvec2 mGlobalUVOffsetMin = { 0.f, 0.f }, fvec2 mGlobalUVOffsetMax = { 1.f, 1.f });
		GHUD_API void BeginPanel(const Element::Panel& mPanel);
		GHUD_API void EndPanel();
	private:
		void Draw(DrawInfo drawInfo);
		Context* ctx;

		std::multiset<DrawInfo> mDrawList; // ordered version of the vector, based on the layers

		// push stacks
		struct StackPushTransform {
			LayerIndex mLayerOffset{};
			Math::Transform2x2 mTransform{};
			fvec2 mPosition{0.0, 0.0};
			fvec2 mAnchorOffset{ 0.0, 0.0 };

			// to modify panel "size" 
			fvec2 mAnchorAreaLimMin{ -1.0, -1.0 };
			fvec2 mAnchorAreaLimMax{ 1.0, 1.0 };
		};
		StackDrawData<StackPushTransform> mStackTransform;

		size_t mPreviousSize = 0;
	};
}