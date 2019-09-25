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
				if (mMusic.IsPlaying()) {
					mCurrentTime = mMusic.GetCurrentPosition() / mMusic.GetLengthSeconds();
				}
				if (ImGui::SliderFloat("##apa", &mCurrentTime, 0.0f, 1.0f, "")) {
					mMusic.Seek(mCurrentTime * mMusic.GetLengthSeconds());
					mMusic.Pause();
				}
			}
		}
		ImGui::End();
	}
}

}  // namespace ty