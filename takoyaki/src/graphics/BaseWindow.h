#pragma once

namespace ty {

template <typename DERIVED_TYPE>
class BaseWindow {
public:
	static void WindowInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto userWindow = reinterpret_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			userWindow->OnInput(key, scancode, action, mods);
		} else {
			throw std::runtime_error("Failed to process window input callback");
		}
	}

	static void WindowFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		auto userWindow = static_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			userWindow->OnFramebufferSize(width, height);
		} else {
			throw std::runtime_error("Failed to process window framebuffer size callback");
		}
	}

public:
	virtual void OnInput(int key, int scancode, int action, int mods) = 0;
	virtual void OnFramebufferSize(int width, int height)             = 0;

	GLFWwindow* GetHandle() {
		return mWindow;
	}

protected:
	GLFWwindow* mWindow;
};

}  // namespace ty