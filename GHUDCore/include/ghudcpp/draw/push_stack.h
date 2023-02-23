#pragma once

#include <vector>

namespace GHUD {
	template<typename T>
	class StackDrawData {
		struct StackPushData {
			uint32_t mPushID;
			T mBase;
		};
	public:
		void Push(const T& pushData, const T& applyData) {
			mStack.push_back({ static_cast<uint32_t>(mStack.size()), pushData });
			mBaseApply = applyData;
		}
		void Pop(const T& newApplyData) {
			mStack.pop_back();
			mBaseApply = newApplyData;
		}
		const T& GetBackData() {
			return mStack.back().mBase;
		}
		const T& GetApplyData() {
			return mBaseApply;
		}
		bool HasStack() {
			return mStack.size() != 0;
		}
	private:
		T mBaseApply{};
		std::vector<StackPushData> mStack{};
	};
}