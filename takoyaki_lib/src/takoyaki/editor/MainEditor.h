#pragma once

namespace ty {
class Camera;
class Preview;
class UniformsMenu;
class DockSpace;
class Animator;
class Timeline;
}  // namespace ty

namespace ty {

class MainEditor {
public:
	MainEditor(Camera& camera,
	           Preview& preview,
	           UniformsMenu& uniformsMenu,
	           DockSpace& dockSpace,
	           Animator& animator,
	           Timeline& timeline)
	    : mCamera(camera)
	    , mPreview(preview)
	    , mUniformsMenu(uniformsMenu)
	    , mDockSpace(dockSpace)
	    , mAnimator(animator)
	    , mTimeline(timeline) {}
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

private:
	void DisplayErrors();

	bool mShowDemoWindow = false;
	bool mCameraMode     = false;

	ImVec2 mMenuBarSize;
	glm::ivec2 mFramebufferSize;
	glm::vec2 mContentScale;
	std::vector<std::string> mErrors;

	UniformsMenu& mUniformsMenu;
	Preview& mPreview;
	Camera& mCamera;
	DockSpace& mDockSpace;
	Animator& mAnimator;
	Timeline& mTimeline;

	OpenFileHandler mNewFileHandler;
	OpenFileHandler mOpenFileHandler;
	SaveFileHandler mSaveFileHandler;

	CameraCaptureInputHandler mCameraCaptureHandler;
	CameraReleaseInputHandler mCameraReleaseHandler;
};

}  // namespace ty