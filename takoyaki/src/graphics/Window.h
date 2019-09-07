#pragma once

namespace ty {

class Window {
public:
	Window(int width, int height, const char* title);
	~Window();

	bool ShouldClose() const;
	void RequestClose();

	void InputCallback(int key, int scancode, int action, int mods);
	void SwapBuffers();

	void PollEvents();

	GLFWwindow* GetHandle() {
		return mWindow;
	}

private:
	GLFWwindow* mWindow;

};

}  // namespace ty