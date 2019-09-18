#pragma once

#include "components/Camera.h"
#include "components/Preview.h"
#include "components/UniformsMenu.h"

namespace ty {

class MainEditor {
public:
	MainEditor()  = default;
	~MainEditor() = default;

	void LoadProjectFile(const std::string& fileToLoad);

	void Update(float deltaTime, bool hasProjectLoaded, const RenderTarget& renderTarget);
	void RegisterCommands(RenderCommandList<RenderCommand>& cmds, const ShaderProgram& program);

	void OnInput(const KeyInput& input);
	void OnInput(const MouseInput& input);
	void OnInput(const CursorInput& input);

	void OnFramebufferSize(const glm::ivec2& size);
	void OnContentScale(const glm::vec2& scale);

	void ReportError(const std::string& message);

	void SetNewFileHandler(NewFileHandler handler) { mNewFileHandler = handler; }
	void SetOpenFileHandler(OpenFileHandler handler) { mOpenFileHandler = handler; }
	void SetSaveFileHandler(SaveFileHandler handler) { mSaveFileHandler = handler; }
	void SetCameraCaptureInputHandler(CameraCaptureInputHandler handler) { mCameraCaptureHandler = handler; }
	void SetCameraReleaseInputHandler(CameraReleaseInputHandler handler) { mCameraReleaseHandler = handler; }

	UniformsMenu& GetUniformsMenu() { return mUniformsMenu; }
	Camera& GetCamera() { return mCamera; }

private:
	void DisplayErrors();

	bool mShowDemoWindow = false;
	bool mCameraMode     = false;

	ImVec2 mMenuBarSize;
	glm::ivec2 mFramebufferSize;
	glm::vec2 mContentScale;
	std::vector<std::string> mErrors;

	UniformsMenu mUniformsMenu;
	Preview mPreview;
	Camera mCamera;

	OpenFileHandler mNewFileHandler;
	OpenFileHandler mOpenFileHandler;
	SaveFileHandler mSaveFileHandler;

	CameraCaptureInputHandler mCameraCaptureHandler;
	CameraReleaseInputHandler mCameraReleaseHandler;
};

}  // namespace ty