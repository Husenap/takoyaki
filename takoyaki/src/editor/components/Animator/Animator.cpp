#include "Animator.h"

#include "../../systems/music/MusicSystem.h"

namespace ty {

const char* AnimatorWindowName = "Animator";
const float ColumnWidth        = 200.f;

Animator::Animator(MusicSystem& musicSystem)
    : mMusic(musicSystem) {
	mNumBars        = 112;
	mNumBeats       = mNumBars * 4;
	mNumTicks       = mNumBeats * 4;
	mBPM            = 129.f;
	mSecondsPerTick = (60.f / (mBPM * 4.f));
	mOffset         = 6.951f;

	mAnimationTracks.push_back(AnimationTrack("Scene", UniformType::Float));
	mAnimationTracks.push_back(AnimationTrack("Camera 1", UniformType::Vec3));
	mAnimationTracks.push_back(AnimationTrack("Camera 2", UniformType::Vec3));
	mAnimationTracks.push_back(AnimationTrack("Chromatic Aberration", UniformType::Float));
	mAnimationTracks.push_back(AnimationTrack("Depth of Field", UniformType::Float));
	mAnimationTracks.push_back(AnimationTrack("Something Cool", UniformType::Vec2));
	mAnimationTracks.push_back(AnimationTrack("Something More Cool", UniformType::Vec3));
	mAnimationTracks.push_back(AnimationTrack("Something Even Cool", UniformType::Vec4));
	mAnimationTracks.push_back(AnimationTrack("Something Super Cool", UniformType::Color));
	mAnimationTracks.push_back(AnimationTrack("Dream big", UniformType::Mat4));
}

Animator::~Animator() {}

void Animator::Update() {
	if (mVisibility) {
		if (mMusic.IsLoaded()) {
			mTick = CalcTickFromSeconds(mMusic.GetCurrentPosition());
			mTick = std::clamp<int>(mTick, 0, mNumTicks - 1);
		}
		if (ImGui::Begin(AnimatorWindowName, &mVisibility)) {
			ImGui::BeginChild("##TimelineColumn",
			                  {50.f, 0.f},
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
				mTick = std::clamp<int>(mTick - 16, 0, mNumTicks - 1);
				SyncMusicToTick();
			}
			if (input.key == GLFW_KEY_D) {
				mTick = std::clamp<int>(mTick + 16, 0, mNumTicks - 1);
				SyncMusicToTick();
			}
		} else {
			if (input.key == GLFW_KEY_H) {
				mTrackIndex = std::clamp<int>(mTrackIndex - 1, 0, mAnimationTracks.size() - 1);
			}
			if (input.key == GLFW_KEY_L) {
				mTrackIndex = std::clamp<int>(mTrackIndex + 1, 0, mAnimationTracks.size() - 1);
			}
			if (input.key == GLFW_KEY_K) {
				mTick = std::clamp<int>(mTick - 1, 0, mNumTicks - 1);
				SyncMusicToTick();
			}
			if (input.key == GLFW_KEY_J) {
				mTick = std::clamp<int>(mTick + 1, 0, mNumTicks - 1);
				SyncMusicToTick();
			}
			if (input.key == GLFW_KEY_I) {
				mMusic.Pause();
				mAnimationTracks[mTrackIndex].InsertKey(mTick);
			}
		}
	}
}

void Animator::DrawAnimationTracks() {
	static ImVec4 thing;

	for (int trackIndex = 0; trackIndex < mAnimationTracks.size(); trackIndex++) {
		ImGui::PushID(trackIndex);
		if (trackIndex > 0) ImGui::SameLine();

		AnimationTrack& track = mAnimationTracks[trackIndex];

		ImGui::BeginGroup();
		if (trackIndex == mTrackIndex) {
			ImGui::TextColored(ImVec4(0, 1, 1, 1), track.GetName().c_str());
			ImGui::SetScrollHereX();
		} else {
			ImGui::Text(track.GetName().c_str());
		}

		if (mTrackIndex == trackIndex) {
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.290f, 0.290f, 0.290f, 1.000f));
		}
		ImGuiWindowFlags child_flags = ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
		ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)trackIndex), ImVec2(ColumnWidth, 0.f), true, child_flags);
		for (int currentTick = 0; currentTick < mNumTicks; currentTick++) {
			ImGui::PushID(currentTick);
			if (currentTick == mTick){
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.420f, 0.311f, 0.156f, 0.541f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.420f, 0.311f, 0.156f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.561f, 0.416f, 0.209f, 0.670));
			}
			track.DrawIndex(currentTick);
			if (currentTick == mTick) {
				ImGui::SetScrollHereY();
				ImGui::PopStyleColor(3);
			}
			ImGui::PopID();
		}
		if (mTrackIndex == trackIndex) {
			ImGui::PopStyleColor();
		}
		ImGui::EndChild();
		ImGui::EndGroup();
		ImGui::PopID();
	}
}

void Animator::DrawTimeline() {
	ImGui::Text("Beat");

	ImGui::BeginChild(
	    "##ScrollArea", {0.f, 0.f}, true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
	for (int i = 0; i < mNumTicks; ++i) {
		if (i == mTick) {
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%02X:%02X", i / 16, i % 16);
			ImGui::SetScrollHereY();
		} else {
			ImGui::Text("%02X:%02X", i / 16, i % 16);
		}
	}
	ImGui::EndChild();
}

int Animator::CalcTickFromSeconds(float seconds) {
	return (int)std::roundf((seconds-mOffset) / mSecondsPerTick);
}

float Animator::CalcSecondsFromTick(int tick) {
	return ((float)tick * mSecondsPerTick) + mOffset;
}

void Animator::SyncMusicToTick() {
	mMusic.Pause();
	mMusic.Seek(CalcSecondsFromTick(mTick));
}

}  // namespace ty