#include "MainEditor.h"

namespace ty {

void MainEditor::Update() {
	if (mShowDemoWindow) {
		ImGui::ShowDemoWindow(&mShowDemoWindow);
	}

	if (ImGui::BeginMainMenuBar()) {
		mMenuBarSize = ImGui::GetWindowSize();
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open")) {
				tinyfd_openFileDialog("Open TakoYaki file", "", 0, nullptr, nullptr, 0);
			}
			if (ImGui::MenuItem("Save As..")) {
				tinyfd_saveFileDialog("Save TakoYaki file", "", 0, nullptr, nullptr);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (mShowWorkspace) {
		DrawWorkspace();
	}
}

void MainEditor::DrawWorkspace() {
	if (ImGui::Begin("Workspace", &mShowWorkspace, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus)) {
		ImGui::SetWindowPos({0.f, mMenuBarSize.y}, ImGuiCond_Always);
		ImGui::SetWindowSize({mMenuBarSize.x, mFramebufferSize.y - mMenuBarSize.y}, ImGuiCond_Always);
	}
	ImGui::End();
}

void MainEditor::OnInput(const KeyInput& input) {
	if (input.key == GLFW_KEY_F1 && input.action == GLFW_PRESS) {
		mShowWorkspace = !mShowWorkspace;
	}
}

void MainEditor::OnFramebufferSize(const glm::ivec2& size) {
	mFramebufferSize = size;
}

}  // namespace ty