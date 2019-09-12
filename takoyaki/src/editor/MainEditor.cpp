#include "MainEditor.h"

namespace ty {

const char* ErrorPopupName         = "Error##Popup";

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

	mUniformsMenu.Update();

	if (!mErrors.empty()) {
		DisplayErrors();
	}
}

void MainEditor::OnInput(const KeyInput& input) {
	if (input.key == GLFW_KEY_F1 && input.action == GLFW_PRESS) {
		mUniformsMenu.ToggleVisibility();
	}
	if (input.key == GLFW_KEY_F4 && input.action == GLFW_PRESS) {
		mShowDemoWindow = !mShowDemoWindow;
	}
}

void MainEditor::OnFramebufferSize(const glm::ivec2& size) {
	mFramebufferSize = size;
}

void MainEditor::OnContentScale(const glm::vec2& scale) {
	mContentScale = scale;
}

void MainEditor::ReportError(const std::string& message) {
	mErrors.emplace_back(message);
}

void MainEditor::DisplayErrors() {
	if (!ImGui::IsPopupOpen(ErrorPopupName)) {
		ImGui::OpenPopup(ErrorPopupName);
	}
	if (ImGui::BeginPopupModal(ErrorPopupName, nullptr, ImGuiWindowFlags_NoResize)) {
		ImGui::SetWindowSize({300.f*mContentScale.x, 0.f});
		ImGui::TextWrapped(mErrors.front().c_str());
		if (ImGui::Button("Ok", {-FLT_EPSILON, 0.0})) {
			mErrors.erase(mErrors.begin());
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void MainEditor::RegisterCommands(RenderCommandList<RenderCommand>& cmds, std::unique_ptr<ShaderProgram>& program) {
	mUniformsMenu.RegisterCommands(cmds, program);
}

}  // namespace ty