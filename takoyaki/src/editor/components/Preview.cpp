#include "Preview.h"

namespace ty {

void Preview::Update(const RenderTarget& renderTarget) {
	if (mVisibility) {
		if (ImGui::Begin("Preview", &mVisibility)) {
			mIsHovered = ImGui::IsWindowHovered();
			ImGui::Image((void*)(intptr_t)renderTarget.GetRenderTexture(), renderTarget.GetSize(), {0, 1}, {1, 0});
		}
		ImGui::End();
	}
}

}  // namespace ty
