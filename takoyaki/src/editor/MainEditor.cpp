#include "MainEditor.h"

namespace ty {

const char* ErrorPopupName = "Error##Popup";

void MainEditor::Update() {
	if (mShowDemoWindow) {
		ImGui::ShowDemoWindow(&mShowDemoWindow);
	}

	if (ImGui::BeginMainMenuBar()) {
		mMenuBarSize = ImGui::GetWindowSize();
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open", "Ctrl + O")) {
				mOpenFileHandler();
			}
			if (ImGui::MenuItem("Save", "Ctrl + S")) {
				mSaveFileHandler();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Uniforms", "F1")) {
				mUniformsMenu.ToggleVisibility();
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

void MainEditor::RegisterCommands(RenderCommandList<RenderCommand>& cmds, std::unique_ptr<ShaderProgram>& program) {
	mUniformsMenu.RegisterCommands(cmds, program);
}

void MainEditor::OnInput(const KeyInput& input) {
	if (input.key == GLFW_KEY_S && (input.mods == GLFW_MOD_CONTROL)) {
		mSaveFileHandler();
	}
	if (input.key == GLFW_KEY_O && input.mods == GLFW_MOD_CONTROL) {
		mOpenFileHandler();
	}

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
		ImGui::SetWindowSize({300.f * mContentScale.x, 0.f});
		ImGui::TextWrapped(mErrors.front().c_str());
		if (ImGui::Button("Ok", {-FLT_EPSILON, 0.0})) {
			mErrors.erase(mErrors.begin());
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void MainEditor::OpenFile(std::string_view file) {
	mUniformsMenu.OpenFile(file);
}

void MainEditor::SaveFile(std::string_view file) {
	mUniformsMenu.SaveFile(file);
}

}  // namespace ty