#include "MainEditor.h"

namespace ty {

void MainEditor::Update() {
	if (mShowDemoWindow) {
		ImGui::ShowDemoWindow(&mShowDemoWindow);
	}

	if (ImGui::BeginMainMenuBar()) {
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
	if (ImGui::Begin("Workspace", &mShowWorkspace, ImGuiWindowFlags_NoDecoration)) {
	}
	ImGui::End();
}

}  // namespace ty