#include "MainEditor.h"

#include "components/Animator/Animator.h"
#include "components/Camera.h"
#include "components/DockSpace.h"
#include "components/Preview.h"
#include "components/Timeline/Timeline.h"
#include "components/UniformsMenu.h"
#include "systems/project/ProjectSystem.h"
#include "systems/scene/SceneSystem.h"

namespace ty {

void MainEditor::Update(float deltaTime, const RenderTarget& renderTarget) {
	mDockSpace.Update();

	bool hasProjectLoaded = mProjectSystem.IsProjectLoaded();

	if (mShowDemoWindow) {
		ImGui::ShowDemoWindow(&mShowDemoWindow);
	}

	if (ImGui::BeginMainMenuBar()) {
		mMenuBarSize = ImGui::GetWindowSize();
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New", "Ctrl + N")) {
				mProjectSystem.NewProject();
			}
			if (ImGui::MenuItem("Open", "Ctrl + O")) {
				mProjectSystem.OpenProject();
			}
			if (ImGui::MenuItem("Save", "Ctrl + S", nullptr, hasProjectLoaded)) {
				mProjectSystem.SaveProject();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Project")) {
			if (ImGui::MenuItem("New Scene")) {
				mSceneSystem.NewScene();
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
		mPreview.Update(renderTarget);
		mCamera.Update(deltaTime);
	}
	mTimeline.Update();
	mAnimator.Update();
}

void MainEditor::OnInput(const KeyInput& input) {
	mAnimator.OnInput(input);
	mCamera.ProcessKeyInput(input);

	if (mCameraMode) {
		return;
	}

	if (input.key == GLFW_KEY_N && input.mods == GLFW_MOD_CONTROL) {
		mProjectSystem.NewProject();
	}
	if (input.key == GLFW_KEY_O && input.mods == GLFW_MOD_CONTROL) {
		mProjectSystem.OpenProject();
	}
	if (input.key == GLFW_KEY_S && (input.mods == GLFW_MOD_CONTROL)) {
		mProjectSystem.SaveProject();
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

}  // namespace ty