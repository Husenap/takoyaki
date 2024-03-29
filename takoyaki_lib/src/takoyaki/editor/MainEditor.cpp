#include "MainEditor.h"

#include "components/Animator/Animator.h"
#include "components/Camera.h"
#include "components/DockSpace.h"
#include "components/Preview.h"
#include "components/Timeline/Timeline.h"
#include "components/UniformsMenu.h"

namespace ty {

const char* ErrorPopupName = "Error##Popup";

void MainEditor::LoadProjectFile(const std::string& fileToLoad) {
	mUniformsMenu.OpenFile(fileToLoad);
	mCamera.Reset();
}

void MainEditor::Update(float deltaTime, bool hasProjectLoaded, const RenderTarget& renderTarget) {
	mDockSpace.Update();

	if (mShowDemoWindow) {
		ImGui::ShowDemoWindow(&mShowDemoWindow);
	}

	if (ImGui::BeginMainMenuBar()) {
		mMenuBarSize = ImGui::GetWindowSize();
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New", "Ctrl + N")) {
				mNewFileHandler();
			}
			if (ImGui::MenuItem("Open", "Ctrl + O")) {
				mOpenFileHandler();
			}
			if (ImGui::MenuItem("Save", "Ctrl + S", nullptr, hasProjectLoaded)) {
				mSaveFileHandler();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, false)) {
			}
			if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z", nullptr, false)) {
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Uniforms", "F1", nullptr, hasProjectLoaded)) {
				mUniformsMenu.ToggleVisibility();
			}
			if (ImGui::MenuItem("Preview", "F2", nullptr, hasProjectLoaded)) {
				mPreview.ToggleVisibility();
			}
			if (ImGui::MenuItem("Animator", "F3", nullptr, hasProjectLoaded)) {
				mAnimator.ToggleVisibility();
			}
			if (ImGui::MenuItem("Timeline", "F4", nullptr, hasProjectLoaded)) {
				mTimeline.ToggleVisibility();
			}
			if (ImGui::MenuItem("Camera", "F5", nullptr, hasProjectLoaded)) {
				mCamera.ToggleVisibility();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (hasProjectLoaded) {
		mUniformsMenu.Update();
		mPreview.Update(renderTarget);
		mCamera.Update(deltaTime);
	}
	mTimeline.Update();
	mAnimator.Update();

	if (!mErrors.empty()) {
		DisplayErrors();
	}
}

void MainEditor::RegisterCommands(RenderCommandList<RenderCommand>& cmds, const ShaderProgram& program) {
	mUniformsMenu.RegisterCommands(cmds, program);
}

void MainEditor::OnInput(const KeyInput& input) {
	mAnimator.OnInput(input);
	mCamera.ProcessKeyInput(input);

	if (mCameraMode) {
		return;
	}

	if (input.key == GLFW_KEY_N && input.mods == GLFW_MOD_CONTROL) {
		if (mNewFileHandler) mNewFileHandler();
	}
	if (input.key == GLFW_KEY_O && input.mods == GLFW_MOD_CONTROL) {
		if (mOpenFileHandler) mOpenFileHandler();
	}
	if (input.key == GLFW_KEY_S && (input.mods == GLFW_MOD_CONTROL)) {
		if (mSaveFileHandler) mSaveFileHandler();
	}

	if (input.key == GLFW_KEY_F1 && input.action == GLFW_PRESS) {
		mUniformsMenu.ToggleVisibility();
	}
	if (input.key == GLFW_KEY_F2 && input.action == GLFW_PRESS) {
		mPreview.ToggleVisibility();
	}
	if (input.key == GLFW_KEY_F3 && input.action == GLFW_PRESS) {
		mAnimator.ToggleVisibility();
	}
	if (input.key == GLFW_KEY_F4 && input.action == GLFW_PRESS) {
		mTimeline.ToggleVisibility();
	}
	if (input.key == GLFW_KEY_F5 && input.action == GLFW_PRESS) {
		mCamera.ToggleVisibility();
	}
	if (input.key == GLFW_KEY_F6 && input.action == GLFW_PRESS) {
		mShowDemoWindow = !mShowDemoWindow;
	}
}

void MainEditor::OnInput(const MouseInput& input) {
	if (input.button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (mPreview.IsHovered() && input.action == GLFW_PRESS) {
			mCameraMode = true;
			mCamera.SetActive(true);
			if (mCameraCaptureHandler) mCameraCaptureHandler();
		} else if (input.action == GLFW_RELEASE) {
			mCameraMode = false;
			mCamera.SetActive(false);
			if (mCameraReleaseHandler) mCameraReleaseHandler();
		}
	}
}

void MainEditor::OnInput(const CursorInput& input) {
	if (mCameraMode) {
		mCamera.ProcessMouseMovement(input.delta);
	}
}

void MainEditor::OnFramebufferSize(const glm::ivec2& size) {
	mFramebufferSize = size;
}

void MainEditor::OnContentScale(const glm::vec2& scale) {
	mContentScale = scale;
}

void MainEditor::ReportError(const std::string& message) {
	// mErrors.emplace_back(message);
	tinyfd_notifyPopup("Error!", message.c_str(), "error");
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

}  // namespace ty