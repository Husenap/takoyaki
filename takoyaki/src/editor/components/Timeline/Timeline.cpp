#include "Timeline.h"

#include "../../systems/music/MusicSystem.h"

namespace ty {

const char* TimelineWindowName = "Timeline";
const float ScrubberHeight     = 100.f;

Timeline::Timeline(MusicSystem& musicSystem)
    : mMusic(musicSystem)
    , mCurrentTime(0.f) {}

Timeline::~Timeline() {}

void Timeline::Update() {
	std::string title = TimelineWindowName;
	if (mMusic.IsLoaded()) {
		title += " (" + std::to_string(mMusic.GetCurrentPosition()) + ")";
	}
	title += "###Timeline";
	if (Begin(title.c_str())) {
		ImGui::SetNextItemWidth(-FLT_EPSILON);

		if (mMusic.IsLoaded()) {
			mCurrentTime = mMusic.GetCurrentPosition() / mMusic.GetLengthSeconds();

			DrawVolumeControls();

			ImGui::SameLine();

			ImGui::BeginChild("##TimelineContainer", {0.f, 0.f});
			DrawPlayPauseButton();
			DrawScrubber();
			ImGui::EndChild();
		}
	}
	End();
}

void Timeline::DrawPlayPauseButton() {
	const char* playPauseButtonTitle = mMusic.IsPlaying() ? "P\nA\nU\nS\nE" : "P\nL\nA\nY";
	if (ImGui::Button(playPauseButtonTitle)) {
		if (mMusic.IsPlaying()) {
			mMusic.Pause();
		} else {
			mMusic.Play();
		}
	}
}

void Timeline::DrawVolumeControls()
{
	ImGui::BeginChild("##VolumeContainer", {25.f, ScrubberHeight});
	static float volume = 100.f;
	if (ImGui::VSliderFloat("##VolumeSlider", {20.f, ScrubberHeight}, &volume, 0.f, 100.f, "")) {
		mMusic.SetVolume(volume / 100.f);
}
	if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Volume: %.0f%%", volume);
	}
	ImGui::EndChild();
}

void Timeline::DrawScrubber() {
	ImDrawList& drawList = *ImGui::GetWindowDrawList();
	const ImVec2 p       = ImGui::GetCursorScreenPos();
	float widthAvailable = ImGui::GetContentRegionAvailWidth();
	ImGui::SetNextItemWidth(-FLT_EPSILON);
	ImGui::PlotHistogram(
	    "##AudioSpectrum", mMusic.GetPeaks().data(), mMusic.GetPeaks().size(), 0, "", 0.f, 1.f, {0.f, ScrubberHeight});
	if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0)) {
		mCurrentTime = (ImGui::GetIO().MousePos.x - p.x) / widthAvailable;
		mMusic.Seek(mCurrentTime * mMusic.GetLengthSeconds());
		mMusic.Pause();
	}
	float xPos = widthAvailable * mCurrentTime;
	drawList.AddLine({p.x + xPos, p.y-1.f}, {p.x + xPos, p.y - 1.f + ScrubberHeight}, 0xee1111ff, 2.0f);
}

}  // namespace ty