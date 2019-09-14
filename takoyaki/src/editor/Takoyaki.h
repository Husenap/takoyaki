#pragma once

#include "../graphics/MainWindow.h"
#include "../graphics/Renderer.h"
#include "MainEditor.h"

namespace ty {

class Takoyaki {
public:
	Takoyaki();
	~Takoyaki();

private:
	void OnInput(const KeyInput& input);

	void OnFramebufferSize(const glm::ivec2& size);
	void OnContentScale(const glm::vec2& scale);
	void CreateVertexBuffer();
	void CreateRenderTarget();
	void SetupListeners();

	void LoadShader();

	void OnNewFile();
	void OnOpenFile();
	void OnSaveFile();

	void LoadProjectFile(const char* fileToLoad);

	Renderer mRenderer;
	MainWindow mWindow;
	MainEditor mEditor;
	FileWatcher mFileWatcher;

	GLuint mVertexArray;
	std::unique_ptr<ShaderProgram> mProgram;
	GLint mPosLoc;
	GLint mFrameLoc;
	GLint mTimeLoc;
	GLint mResolutionLoc;
	GLint mCameraOriginLoc;
	GLint mCameraTargetLoc;

	std::unique_ptr<RenderTarget> mRenderTarget;

	std::string mCurrentProject;
	void OnUniformsChanged();
};

}  // namespace ty