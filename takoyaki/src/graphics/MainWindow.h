#pragma once

#include "BaseWindow.h"

namespace ty {

class MainWindow : public BaseWindow<MainWindow> {
public:
	MainWindow(int width, int height, const char* title);
	~MainWindow();

	bool ShouldClose() const;
	void RequestClose();

	void SwapBuffers();

	void PollEvents();

	const glm::ivec2& GetFramebufferSize() const;

	virtual void OnInput(int key, int scancode, int action, int mods) override;
	virtual void OnFramebufferSize(glm::ivec2 size) override;

private:
	void InitCallbacks();
	void InitGL();
	void InitImGui();

	glm::ivec2 mFramebufferSize;
};

}  // namespace ty