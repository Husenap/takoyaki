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

	virtual void OnInput(int key, int scancode, int action, int mods) override;
	virtual void OnFramebufferSize(int width, int height) override;
};

}  // namespace ty