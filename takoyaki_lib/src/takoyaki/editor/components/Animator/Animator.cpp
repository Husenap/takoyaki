#include "Animator.h"

#include "../../systems/animator/AnimationSystem.h"
#include "../../systems/music/MusicSystem.h"
#include "../../systems/sync/SyncSystem.h"

namespace ty {

const char* AnimatorWindowName = "Animator";
const float ColumnWidth        = 100.f;
const int TicksPerBar          = 16;
const char* TickFormat         = "%02X:%01X";

Animator::Animator(MusicSystem& musicSystem, AnimationSystem& animationSystem, SyncSystem& syncSystem)
    : mMusic(musicSystem)
    , mAnimationSystem(animationSystem)
    , mSyncSystem(syncSystem) {
	mSyncSystem.SetBars(114);
	mSyncSystem.SetBPM(129.f);
	mSyncSystem.SetOffset(6.951f - mSyncSystem.TickToSecondsWithoutOffset(32));
}

Animator::~Animator() {}

void Animator::Update() {
	if (mMusic.IsLoaded()) {
		mTick = mSyncSystem.SecondsToTick(mMusic.GetCurrentPosition());
		mTick = std::clamp<int>(mTick, 0, mSyncSystem.NumTicks() - 1);
	}
	if (Begin(AnimatorWindowName)) {
		ImGui::BeginChild(
		    "##TimelineColumn", {50.f, 0.f}, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		DrawTimeline();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("##HorizontalScroller", {0.f, 0.f}, false, ImGuiWindowFlags_NoScrollWithMouse);
		DrawAnimationTracks();
		ImGui::EndChild();
	}
	End();
}

void Animator::OnInput(const KeyInput& input) {
	if (!mIsFocused) {
		return;
	}

	if (input.action != GLFW_RELEASE) {
		if (input.mods & GLFW_MOD_CONTROL) {
			if (input.key == GLFW_KEY_U) {
				MoveTickIndex(-16);
			}
			if (input.key == GLFW_KEY_D) {
				MoveTickIndex(16);
			}
		} else {
			if (input.key == GLFW_KEY_H) {
				MoveTrackIndex(-1);
			}
			if (input.key == GLFW_KEY_L) {
				MoveTrackIndex(1);
			}
			if (input.key == GLFW_KEY_K) {
				MoveTickIndex(-1);
			}
			if (input.key == GLFW_KEY_J) {
				MoveTickIndex(1);
			}
			if (input.key == GLFW_KEY_I) {
				mMusic.Pause();
				mAnimationSystem.GetAnimationTracks()[mTrackIndex].InsertKey(mTick);
			}
			if (input.key == GLFW_KEY_D) {
				mMusic.Pause();
				mAnimationSystem.GetAnimationTracks()[mTrackIndex].RemoveKey(mTick);
			}
			if (input.key == GLFW_KEY_E) {
				mMusic.Pause();
				mAnimationSystem.GetAnimationTracks()[mTrackIndex].ToggleEasingType(mTick);
			}
		}
	}
}

void Animator::DrawAnimationTracks() {
	static ImVec4 thing;

	auto& animationTracks = mAnimationSystem.GetAnimationTracks();
	for (int trackIndex = 0; trackIndex < animationTracks.size(); trackIndex++) {
		ImGui::PushID(trackIndex);
		if (trackIndex > 0) ImGui::SameLine();

		AnimationTrack& track = animationTracks[trackIndex];

		ImGui::BeginGroup();
		if (trackIndex == mTrackIndex) {
			ImGui::TextColored(ImVec4(0, 1, 1, 1), track.GetName().c_str());
			ImGui::SetScrollHereX();
		} else {
			ImGui::Text(track.GetName().c_str());
		}

		if (mTrackIndex == trackIndex) {
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.32f, 0.32f, 0.32f, 1.000f));
		}
		ImGuiWindowFlags child_flags = ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
		ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)trackIndex), ImVec2(ColumnWidth, 0.f), true, child_flags);
		ImGuiListClipper listClipper;
		listClipper.Begin(mSyncSystem.NumTicks(), ImGui::GetTextLineHeightWithSpacing());
		int firstTickIndex = std::min(listClipper.DisplayStart, mTick);
		int lastTickIndex  = std::max(listClipper.DisplayEnd, mTick + 1);
		for (int currentTick = firstTickIndex; currentTick < lastTickIndex; currentTick++) {
			ImGui::PushID(currentTick);
			if (currentTick == mTick) {
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.420f, 0.311f, 0.156f, 0.541f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.420f, 0.311f, 0.156f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.561f, 0.416f, 0.209f, 0.67f));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.65f, 1.0f, 1.0f));
				ImGui::Separator();
				ImGui::Separator();
			}
			track.DrawTick(currentTick);
			if (currentTick == mTick) {
				ImGui::SetScrollHereY();
				ImGui::Separator();
				ImGui::Separator();
				ImGui::PopStyleColor(4);
			}
			ImGui::PopID();
		}
		listClipper.End();
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
	ImGuiListClipper listClipper;
	listClipper.Begin(mSyncSystem.NumTicks(), ImGui::GetTextLineHeightWithSpacing());
	int firstTickIndex = std::min(listClipper.DisplayStart, mTick);
	int lastTickIndex  = std::max(listClipper.DisplayEnd, mTick + 1);
	for (int currentTick = firstTickIndex; currentTick < lastTickIndex; ++currentTick) {
		int barTime = currentTick / TicksPerBar;
		int tickTime = currentTick % TicksPerBar;
		if (currentTick == mTick) {
			ImGui::TextColored(ImVec4(1, 1, 0, 1), TickFormat, barTime, tickTime);
			ImGui::SetScrollHereY();
		} else {
			ImGui::Text(TickFormat, barTime, tickTime);
		}
	}
	listClipper.End();
	ImGui::EndChild();
}

void Animator::SyncMusicToTick() {
	mMusic.Pause();
	mMusic.Seek(mSyncSystem.TickToSeconds(mTick));
}

void Animator::MoveTrackIndex(int change) {
	mTrackIndex = std::clamp<int>(mTrackIndex + change, 0, mAnimationSystem.GetTrackCount() - 1);
}

void Animator::MoveTickIndex(int change) {
	mTick = std::clamp<int>(mTick + change, 0, mSyncSystem.NumTicks() - 1);
	SyncMusicToTick();
}

}  // namespace ty