#include "Timeline.h"

#include "../../systems/music/MusicSystem.h"

namespace ty {

const char* TimelineWindowName = "Timeline";

Timeline::Timeline(MusicSystem& musicSystem)
    : mMusic(musicSystem)
    , mCurrentTime(0.f) {}

Timeline::~Timeline() {}

void Timeline::Update() {
	if (mVisibility) {
		std::string title = TimelineWindowName;
		if (mMusic.IsLoaded()) {
			title += " (" + std::to_string(mMusic.GetCurrentPosition()) + ")";
		}
		title += "###Timeline";
		if (ImGui::Begin(title.c_str(), &mVisibility)) {
			ImGui::SetNextItemWidth(-FLT_EPSILON);

			if (mMusic.IsLoaded()) {
				mCurrentTime = mMusic.GetCurrentPosition() / mMusic.GetLengthSeconds();

				const char* playPauseButtonTitle = mMusic.IsPlaying() ? "Pause" : "Play";
				if (ImGui::Button(playPauseButtonTitle)) {
					if (mMusic.IsPlaying()) {
						mMusic.Pause();
					} else {
						mMusic.Play();
					}
				}

				ImGui::SetNextItemWidth(-FLT_EPSILON);
				if (ImGui::SliderFloat("##CurrentTime", &mCurrentTime, 0.0f, 1.0f, "")) {
					mMusic.Seek(mCurrentTime * mMusic.GetLengthSeconds());
					mMusic.Pause();
				}

				static float volume = 100.f;
				if (ImGui::SliderFloat("Volume", &volume, 0.f, 100.f, "%.0f%%")) {
					mMusic.SetVolume(volume / 100.f);
				}
			}
		}
		ImGui::End();
	}
}

}  // namespace ty