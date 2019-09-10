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
		DrawUniformList();
	}
}

void MainEditor::DrawUniformList() {
	static int item = 0;
	static char nameBuffer[64];

	if (ImGui::Begin("Uniforms", &mShowWorkspace)) {
		if (ImGui::Button("Add Uniform")) {
			if (!ImGui::IsPopupOpen("Add New Uniform")) {
				memset(nameBuffer, 0, 64);
				item = 0;
				ImGui::OpenPopup("Add New Uniform");
			}
		}
		for (const auto& uniform : mUniforms) {
			ImGui::Text(uniform.c_str());
		}
	}
	if (ImGui::BeginPopupModal("Add New Uniform", nullptr, ImGuiWindowFlags_NoResize)) {
		ImGui::SetWindowSize({0.f, 0.f});
		ImGui::Combo("Type", &item, "float\0vec2\0vec3\0vec4\0\0");
		ImGui::InputText("Variable Name", nameBuffer, 64);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.f, 0.f));
		ImGui::Columns(2, nullptr, false);
		if (ImGui::Button("Create", {-FLT_EPSILON, 0.0})) {
			mUniforms.push_back(nameBuffer);
			ImGui::CloseCurrentPopup();
		}
		ImGui::NextColumn();
		if (ImGui::Button("Cancel", {-FLT_EPSILON, 0.0})) {
			ImGui::CloseCurrentPopup();
		}
        ImGui::Columns(1);
		ImGui::PopStyleVar();

		ImGui::EndPopup();
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

void MainEditor::OnContentScale(const glm::vec2& scale) {}

}  // namespace ty