#pragma once

namespace ty {

template <typename DERIVED_TYPE>
class BaseWindow {
public:
	using InputCallbackType           = std::function<void(int, int, int, int)>;
	using FramebufferSizeCallbackType = std::function<void(glm::ivec2)>;

public:
	static void WindowInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto userWindow = reinterpret_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			userWindow->OnInput(key, scancode, action, mods);
			for (auto callback : userWindow->mInputListeners) callback(key, scancode, action, mods);
		} else {
			throw std::runtime_error("Failed to process window input callback");
		}
	}

	static void WindowFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		auto userWindow = static_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			auto size = glm::ivec2(width, height);
			userWindow->OnFramebufferSize(size);
			for (auto callback : userWindow->mFramebufferSizeListeners) callback(size);
		} else {
			throw std::runtime_error("Failed to process window framebuffer size callback");
		}
	}

public:
	virtual void OnInput(int key, int scancode, int action, int mods) = 0;
	virtual void OnFramebufferSize(glm::ivec2 size)                   = 0;

	void AddInputListener(InputCallbackType callback) {
		if (callback) mInputListeners.emplace_back(callback);
	}
	void AddFramebufferSizeListener(FramebufferSizeCallbackType callback) {
		if (callback) mFramebufferSizeListeners.emplace_back(callback);
	}

protected:
	GLFWwindow* mWindow;

private:
	std::vector<InputCallbackType> mInputListeners;
	std::vector<FramebufferSizeCallbackType> mFramebufferSizeListeners;
};

}  // namespace ty