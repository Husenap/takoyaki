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
	         UniformsMenu& uniformsMenu,
	         MusicSystem& musicSystem,
	         AnimationSystem& animationSystem,
	         Exporter& exporter);
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
	void OnExportFile();

	void OnCameraCaptureInput();
	void OnCameraReleaseInput();
	void OnUniformsChanged();

	void LoadProjectFile(const char* fileToLoad);

	std::string mCurrentProject;
	float mCurrentTime;

	GLuint mVertexArray;
	std::unique_ptr<ShaderProgram> mProgram;
	GLint mPosLoc;
	GLint mFrameLoc;
	GLint mTimeLoc;
	GLint mResolutionLoc;
	GLint mCameraOriginLoc;
	GLint mCameraTargetLoc;
	std::unique_ptr<RenderTarget> mRenderTarget;

	MainWindow& mWindow;
	Renderer& mRenderer;
	FileWatcher& mFileWatcher;
	MainEditor& mEditor;
	Camera& mCamera;
	UniformsMenu& mUniformsMenu;
	MusicSystem& mMusic;
	AnimationSystem& mAnimationSystem;
	Exporter& mExporter;
};

}  // namespace ty