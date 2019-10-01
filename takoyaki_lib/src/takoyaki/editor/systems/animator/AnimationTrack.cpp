#include "AnimationTrack.h"

namespace ty {

namespace {
ImVec4 EasingToColor(Easings::Type type) {
	switch (type) {
	case Easings::Zero:
		return ImVec4(0.900f, 0.700f, 0.000f, 1.000f);
	case Easings::Linear:
		return ImVec4(1.000f, 0.430f, 0.350f, 1.000f);
	case Easings::Exponential:
		return ImVec4(0.260f, 0.590f, 0.980f, 1.000f);
	case Easings::Smoothstep:
		return ImVec4(0.350f, 1.000f, 0.540f, 1.000f);
	default:
		return ImVec4{1.f, 1.f, 1.f, 1.f};
	}
}
}  // namespace

AnimationTrack::AnimationTrack(const std::string& name, bool isUserTrack)
    : mName(name)
    , mIsUserTrack(isUserTrack) {}

AnimationTrack::~AnimationTrack() {}

void AnimationTrack::DrawTick(int tick) {
	const KeyFramePair pair = mKeys.GetKeyPair(tick);

	float* tickValue = nullptr;
	ImGui::SetNextItemWidth(-FLT_EPSILON);
	if (pair.mStart.mTick == tick && (tickValue = mKeys.GetTickValue(tick))) {
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_Text, EasingToColor(pair.mStart.mEasingType));
		ImGui::DragFloat("", tickValue);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		if (mTickToFocus == tick) {
			ImGui::SetKeyboardFocusHere(-1);
			mTickToFocus = -1;
		}
	} else {
		ImGui::TextColored(EasingToColor(pair.mStart.mEasingType), "---");
	}
}

void AnimationTrack::InsertKey(int tick) {
	mKeys.InsertKey(tick);
}

void AnimationTrack::RemoveKey(int tick) {
	mKeys.RemoveKey(tick);
}

void AnimationTrack::ToggleEasingType(int tick) {
	mKeys.ToggleEasingType(tick);
}

void AnimationTrack::FocusTick(int tick) {
	mTickToFocus = tick;
}

const KeyFramePair AnimationTrack::GetKeyPair(int tick) {
	return mKeys.GetKeyPair(tick);
}

}  // namespace ty
