#include "Timeline.h"

namespace ty {

const char* TimelineWindowName = "Timeline";

Timeline::Timeline() {}

Timeline::~Timeline() {}

void Timeline::Update() {
	static float time = 0.4f;
	if (mVisibility) {
		if (ImGui::Begin(TimelineWindowName, &mVisibility)) {
			ImGui::SetNextItemWidth(-FLT_EPSILON);
			ImGui::SliderFloat(" ", &time, 0.0f, 1.0f);
		}
		ImGui::End();
	}
}

}  // namespace ty