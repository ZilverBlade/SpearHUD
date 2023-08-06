#pragma once

#include <shudcpp/context.h>
#include <shudcpp/draw/element.h>
#include <shudcpp/draw/push_stack.h>
#include <shudcpp/draw/draw_vector.h>
#include <set>
#include <vector>

namespace SHUD {
	class Context; // fwd declare

	class DrawList : virtual public NonCopyableClass {
	public:
		SHUD_API DrawList(Context* ctx);
		SHUD_API ~DrawList();
		SHUD_API void FrameStart();
		SHUD_API void FrameEnd();
		SHUD_API void Clear();
		const std::multiset<DrawInfo>& GetList() {
			return mDrawList;
		}

		// Draw functions

		SHUD_API const Element::Line DrawLine(const Element::Line& line);
		SHUD_API const Element::Line DrawLine(fvec2 mPointA, fvec2 mPointB, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset = { 0.0, 0.0 });
		SHUD_API const Element::Rect DrawRect(const Element::Rect& rect);
		SHUD_API const Element::Rect DrawRect(const Transform& mTransform, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset = { 0.0, 0.0 });
		SHUD_API const Element::Image DrawImage(const Element::Image& img);
		SHUD_API const Element::Image DrawImage(const Transform& mTransform, const TextureObject& mTexture, RGBAColor mColor, LayerIndex mLayer, fvec2 mAnchorOffset = { 0.0, 0.0 }, fvec2 mGlobalUVOffsetMin = { 0.f, 0.f }, fvec2 mGlobalUVOffsetMax = { 1.f, 1.f });
		
		SHUD_API const Element::Text DrawText(const Element::Text& txt);
		SHUD_API const Element::Text DrawText(const Transform& mTransform, RGBAColor mColor, LayerIndex mLayer, const TextFormatting& mFormatting, const FormattedText& mText, fvec2 mAnchorOffset = { 0.0, 0.0 });

		SHUD_API const Element::Button DrawButton(const Element::Button& btn);
		SHUD_API void BeginPanel(const Element::Panel& mPanel);
		SHUD_API void EndPanel();
	private:
		void Draw(DrawInfo drawInfo);
		Context* ctx;

		std::multiset<DrawInfo> mDrawList; // ordered version of the vector, based on the layers

		// push stacks
		struct StackPushTransform {
			LayerIndex mLayerOffset{};
			Math::Transform2x2 mTransform{};
			fvec2 mAbsPosition{0.0, 0.0}; // position [-1, 1]
			fvec2 mAnchorOffset{ 0.0, 0.0 };

			// to modify panel "size" 
			fvec2 mAnchorAreaScale{ 1.0, 1.0 };
		};
		StackDrawData<StackPushTransform> mStackTransform;

		size_t mPreviousSize = 0;
	};
}