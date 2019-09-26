#include "AnimationTrack.h"

namespace ty {

AnimationTrack::AnimationTrack(const std::string& name, UniformType uniformType)
    : mName(name)
    , mUniformType(uniformType) {}

AnimationTrack::~AnimationTrack() {}

void AnimationTrack::DrawIndex(int index) {
	auto keyFrame = mKeyFrames.find(index);

	if (keyFrame == mKeyFrames.end()) {
		ImGui::Text("-- -- --");
		return;
	}

	ImGui::PushItemWidth(-FLT_EPSILON);
	std::visit(make_overloaded{[](auto& item) { item.Update(); }}, keyFrame->second);
	ImGui::PopItemWidth();
}

void AnimationTrack::InsertKey(int index) {
	mKeyFrames[index] = UniformItemFromType(mUniformType);
}

}  // namespace ty
