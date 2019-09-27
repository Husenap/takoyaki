#include "Preview.h"

namespace ty {

void Preview::Update(const RenderTarget& renderTarget) {
	ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.f, 0.f, 0.f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, {0.f, 0.f, 0.f, 1.0f});
	if (Begin("Preview")) {
		mIsHovered = ImGui::IsWindowHovered();

		ImVec2 regionMin  = ImGui::GetWindowContentRegionMin();
		ImVec2 regionMax  = ImGui::GetWindowContentRegionMax();
		ImVec2 offset     = regionMin;
		ImVec2 regionSize = ImVec2(regionMax.x - regionMin.x, regionMax.y - regionMin.y);
		ImVec2 imageSize  = renderTarget.GetSize();

		float regionRatio = regionSize.x / regionSize.y;
		float imageRatio  = (float)renderTarget.GetSize().x / (float)renderTarget.GetSize().y;

		if (regionRatio > imageRatio) {
			imageSize.x *= regionSize.y / imageSize.y;
			imageSize.y = regionSize.y;
		} else {
			imageSize.y *= regionSize.x / imageSize.x;
			imageSize.x = regionSize.x;
		}

		ImGui::SetCursorPosX((regionSize.x - imageSize.x) * 0.5f + offset.x);
		ImGui::SetCursorPosY((regionSize.y - imageSize.y) * 0.5f + offset.y);

		ImGui::Image((void*)(intptr_t)renderTarget.GetRenderTexture(), imageSize, {0, 1}, {1, 0});
	}
	End();
	ImGui::PopStyleColor(2);
}

}  // namespace ty
