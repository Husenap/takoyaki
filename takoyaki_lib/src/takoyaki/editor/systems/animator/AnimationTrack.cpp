#include "AnimationTrack.h"

namespace ty {

namespace {
ImVec4 EasingToColor(Easings::Type type) {
	switch (type) {
	case Easings::Zero:
		return ImVec4{1.f, 1.f, 0.f, 1.f};
	case Easings::Linear:
		return ImVec4{0.f, 1.f, 1.f, 1.f};
	case Easings::Exponential:
		return ImVec4{1.f, 0.f, 1.f, 1.f};
	case Easings::Smoothstep:
		return ImVec4{1.f, 0.f, 0.f, 1.f};
	default:
		return ImVec4{1.f, 1.f, 1.f, 1.f};
	}
}
}  // namespace

AnimationTrack::AnimationTrack(const std::string& name)
    : mName(name) {}

AnimationTrack::~AnimationTrack() {}

void AnimationTrack::DrawTick(int tick) {
	const KeyFramePair pair = mKeys.GetKeyPair(tick);

	ImGui::SetNextItemWidth(-FLT_EPSILON);
	if (pair.mStart.mTick == tick) {
		float* tickValue = mKeys.GetTickValue(tick);
		if (tickValue) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.f, 0.f));
			ImGui::DragFloat("", tickValue);
			ImGui::PopStyleVar();
		}
		else {
			ImGui::TextColored(EasingToColor(pair.mStart.mEasingType), "--");
		}
	} else {
		ImGui::TextColored(EasingToColor(pair.mStart.mEasingType), "%f", pair.mStart.mValue);
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

}  // namespace ty
