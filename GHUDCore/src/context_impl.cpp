#include <ghudcpp/context.h>
#include <ghudcpp/draw/draw_list.h>

namespace GHUD {
	Context::Context() {
		m_DrawList = new DrawList(this);
	}

	Context::~Context() {

	}
	void Context::Pick(fvec2 cursorCoord) {
		m_CtxInfo.m_ScreenSize = cursorCoord;
	}
	void Context::Resize(fvec2 screenSize) {
		m_CtxInfo.m_ScreenSize = screenSize;
		m_CtxInfo.m_AspectRatio = screenSize.x / screenSize.y;
	}
	void Context::Render() {
		//m_DrawDataList.reserve(drawList->GetList().Size());
		//std::vector<SortInfo> sortInfoList{};
		//sortInfoList.reserve(m_DrawDataList.size());
		//for (const auto& element : drawList->GetList()) {
		//	DrawData dd = element.m_Element->GenerateDrawData();
		//	dd.m_ID = element.m_ID;
		//	sortInfoList.emplace_back(SortInfo{ element.m_Element->m_Layer, dd });
		//}
		//mergeSort(sortInfoList.data(), sortInfoList.front(), sortInfoList.back());
	}
}