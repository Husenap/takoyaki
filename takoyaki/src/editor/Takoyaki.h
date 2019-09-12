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

	Renderer mRenderer;
	MainWindow mWindow;
	MainEditor mEditor;
	FileWatcher mFileWatcher;

	GLuint mVertexArray;
	std::unique_ptr<ShaderProgram> mProgram;
	std::unique_ptr<ShaderProgram> mCopyProgram;
	GLint mPosLoc;
	GLint mFrameLoc;
	GLint mTimeLoc;
	GLint mResolutionLoc;

	std::unique_ptr<RenderTarget> mRenderTarget;

	std::string mShaderFileToLoad;
	void CreateCopyProgram();
};

}  // namespace ty