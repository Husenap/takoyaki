#include "Animator.h"

namespace ty {

const char* AnimatorWindowName = "Animator";
const float ColumnWidth        = 250.f;

Animator::Animator() {
	mNumBars  = 108;
	mNumBeats = mNumBars * 4;
	mNumTicks = mNumBeats * 4;
	mAnimationTracks.push_back(AnimationTrack("Scene"));
	mAnimationTracks.push_back(AnimationTrack("Camera 1"));
	mAnimationTracks.push_back(AnimationTrack("Camera 2"));
	mAnimationTracks.push_back(AnimationTrack("Chromatic Aberration"));
	mAnimationTracks.push_back(AnimationTrack("Depth of Field"));
	mAnimationTracks.push_back(AnimationTrack("Something Cool"));
	mAnimationTracks.push_back(AnimationTrack("Something More Cool"));
	mAnimationTracks.push_back(AnimationTrack("Something Even Cool"));
	mAnimationTracks.push_back(AnimationTrack("Something Super Cool"));
	mAnimationTracks.push_back(AnimationTrack("Dream big"));
}

Animator::~Animator() {}

void Animator::Update() {
	if (mVisibility) {
		if (ImGui::Begin(AnimatorWindowName, &mVisibility)) {
			ImGui::BeginChild("##TimelineColumn",
			                  {75.f, 0.f},
			                  false,
			                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			DrawTimeline();
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("##HorizontalScroller", {0.f, 0.f}, false, ImGuiWindowFlags_NoScrollWithMouse);
			DrawAnimationTracks();
			ImGui::EndChild();
		}

		ImGui::End();
	}
}

void Animator::OnInput(const KeyInput& input) {
	if (input.action != GLFW_RELEASE) {
		if (input.mods & GLFW_MOD_CONTROL) {
			if (input.key == GLFW_KEY_U) {
				mY = std::clamp<int>(mY - 16, 0, mNumTicks - 1);
			}
			if (input.key == GLFW_KEY_D) {
				mY = std::clamp<int>(mY + 16, 0, mNumTicks - 1);
			}
		} else {
			if (input.key == GLFW_KEY_H) {
				mX = std::clamp<int>(mX - 1, 0, mAnimationTracks.size() - 1);
			}
			if (input.key == GLFW_KEY_L) {
				mX = std::clamp<int>(mX + 1, 0, mAnimationTracks.size() - 1);
			}
			if (input.key == GLFW_KEY_K) {
				mY = std::clamp<int>(mY - 1, 0, mNumTicks - 1);
			}
			if (input.key == GLFW_KEY_J) {
				mY = std::clamp<int>(mY + 1, 0, mNumTicks - 1);
			}
		}
	}
}

void Animator::DrawAnimationTracks() {
	static ImVec4 thing;

	for (int i = 0; i < mAnimationTracks.size(); i++) {
		if (i > 0) ImGui::SameLine();

		AnimationTrack& track = mAnimationTracks[i];

		ImGui::BeginGroup();
		if (i == mX) {
			ImGui::TextColored(ImVec4(0, 1, 1, 1), track.GetName().c_str());
			ImGui::SetScrollHereX();
		} else {
			ImGui::Text(track.GetName().c_str());
		}

		ImGuiWindowFlags child_flags = ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
		ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)i), ImVec2(ColumnWidth, 0.f), true, child_flags);
		for (int item = 0; item < mNumTicks; item++) {
			if (i == mX && item == mY) {
				ImGui::InputFloat3("", (float*)&thing, 2);
			} else {
				ImGui::Text("- -");
			}
			if (item == mY) {
				ImGui::SetScrollHereY();
			}
		}
		ImGui::EndChild();
		ImGui::EndGroup();
	}
}

void Animator::DrawTimeline() {
	ImGui::Text("Beat");

	ImGui::BeginChild(
	    "##ScrollArea", {0.f, 0.f}, true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
	for (int i = 0; i < mNumTicks; ++i) {
		if (i == mY) {
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%02X:%02X", i / 16, i % 16);
			ImGui::SetScrollHereY();
		} else {
			ImGui::Text("%02X:%02X", i / 16, i % 16);
		}
	}
	ImGui::EndChild();
}

}  // namespace ty