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

	virtual void OnInput(const KeyInput& input) override;
	virtual void OnFramebufferSize(const glm::ivec2& size) override;

private:
	void InitCallbacks();
	void InitGL();
	void InitImGui();
};

}  // namespace ty