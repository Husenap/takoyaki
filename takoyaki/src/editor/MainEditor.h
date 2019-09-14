#pragma once

#include "components/UniformsMenu.h"

namespace ty {

class MainEditor {
public:
	MainEditor()  = default;
	~MainEditor() = default;

	void Update(bool hasProjectLoaded);
	void RegisterCommands(RenderCommandList<RenderCommand>& cmds, std::unique_ptr<ShaderProgram>& program);

	void OnInput(const KeyInput& input);
	void OnFramebufferSize(const glm::ivec2& size);
	void OnContentScale(const glm::vec2& scale);

	void ReportError(const std::string& message);

	void SetNewFileHandler(NewFileHandler handler) { mNewFileHandler = handler; }
	void SetOpenFileHandler(OpenFileHandler handler) { mOpenFileHandler = handler; }
	void SetSaveFileHandler(SaveFileHandler handler) { mSaveFileHandler = handler; }

	UniformsMenu& GetUniformsMenu() { return mUniformsMenu; }

private:
	void DisplayErrors();

	bool mShowDemoWindow = false;

	ImVec2 mMenuBarSize;
	glm::ivec2 mFramebufferSize;
	glm::vec2 mContentScale;
	std::vector<std::string> mErrors;

	UniformsMenu mUniformsMenu;

	OpenFileHandler mNewFileHandler;
	OpenFileHandler mOpenFileHandler;
	SaveFileHandler mSaveFileHandler;
};

}  // namespace ty