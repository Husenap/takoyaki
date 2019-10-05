#pragma once

#include "ServiceManager.h"

namespace ty {

class MainEditor {
public:
	MainEditor(Camera& camera,
	           Preview& preview,
	           DockSpace& dockSpace,
	           Animator& animator,
	           Timeline& timeline,
	           ProjectSystem& projectSystem,
	           SceneSystem& sceneSystem)
	    : mCamera(camera)
	    , mPreview(preview)
	    , mDockSpace(dockSpace)
	    , mAnimator(animator)
	    , mTimeline(timeline)
	    , mProjectSystem(projectSystem)
	    , mSceneSystem(sceneSystem) {}
	~MainEditor() = default;

	void Update(float deltaTime, const RenderTarget& renderTarget);

	void OnInput(const KeyInput& input);
	void OnInput(const MouseInput& input);
	void OnInput(const CursorInput& input);

	void OnFramebufferSize(const glm::ivec2& size);
	void OnContentScale(const glm::vec2& scale);

	void SetCameraCaptureInputHandler(CameraCaptureInputHandler handler) { mCameraCaptureHandler = handler; }
	void SetCameraReleaseInputHandler(CameraReleaseInputHandler handler) { mCameraReleaseHandler = handler; }

private:
	bool mShowDemoWindow = false;
	bool mCameraMode     = false;

	ImVec2 mMenuBarSize;
	glm::ivec2 mFramebufferSize;
	glm::vec2 mContentScale;

	Preview& mPreview;
	Camera& mCamera;
	DockSpace& mDockSpace;
	Animator& mAnimator;
	Timeline& mTimeline;
	ProjectSystem& mProjectSystem;
	SceneSystem& mSceneSystem;

	CameraCaptureInputHandler mCameraCaptureHandler;
	CameraReleaseInputHandler mCameraReleaseHandler;
};

}  // namespace ty