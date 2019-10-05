#pragma once

#include "ServiceManager.h"

namespace ty {

class Takoyaki {
public:
	Takoyaki(MainWindow& window,
	         Renderer& renderer,
	         FileWatcher& fileWatcher,
	         MainEditor& editor,
	         Camera& camera,
	         MusicSystem& musicSystem,
	         AnimationSystem& animationSystem);
	~Takoyaki();

private:
	void OnInput(const KeyInput& input);
	void OnInput(const MouseInput& input);
	void OnInput(const CursorInput& input);

	void OnFramebufferSize(const glm::ivec2& size);
	void OnContentScale(const glm::vec2& scale);
	void CreateVertexBuffer();
	void CreateRenderTarget();
	void SetupListeners();

	void LoadShader();

	void OnNewFile();
	void OnOpenFile();
	void OnSaveFile();

	void OnCameraCaptureInput();
	void OnCameraReleaseInput();

	void LoadProjectFile(const char* fileToLoad);

	float mCurrentTime;

	GLuint mVertexArray;
	std::unique_ptr<ShaderProgram> mProgram;
	std::unique_ptr<RenderTarget> mRenderTarget;

	MainWindow& mWindow;
	Renderer& mRenderer;
	FileWatcher& mFileWatcher;
	MainEditor& mEditor;
	Camera& mCamera;
	MusicSystem& mMusic;
	AnimationSystem& mAnimationSystem;
};

}  // namespace ty