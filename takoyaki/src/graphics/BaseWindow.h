#pragma once

namespace ty {

template <typename DERIVED_TYPE>
class BaseWindow {
public:
	using InputCallbackType           = std::function<void(const KeyInput&)>;
	using FramebufferSizeCallbackType = std::function<void(const glm::ivec2&)>;
	using ContentScaleCallbackType    = std::function<void(const glm::vec2&)>;

public:
	BaseWindow() {}
	virtual ~BaseWindow() {}

	static void WindowInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto userWindow = reinterpret_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			KeyInput input{key, scancode, action, mods};
			userWindow->OnInput(input);
			for (auto callback : userWindow->mInputListeners) callback(input);
		} else {
			throw std::runtime_error("Failed to process window input callback");
		}
	}

	static void WindowFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		auto userWindow = static_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			glm::ivec2 size{width, height};
			userWindow->OnFramebufferSize(size);
			for (auto callback : userWindow->mFramebufferSizeListeners) callback(size);
		} else {
			throw std::runtime_error("Failed to process window framebuffer size callback");
		}
	}

	static void WindowContentScaleCallback(GLFWwindow* window, float scaleX, float scaleY) {
		auto userWindow = static_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			glm::vec2 scale{scaleX, scaleY};
			userWindow->OnContentScale(scale);
			for (auto callback : userWindow->mContentScaleListeners) callback(scale);
		} else {
			throw std::runtime_error("Failed to process window framebuffer size callback");
		}
	}

public:
	virtual void OnInput(const KeyInput& keyInput)         = 0;
	virtual void OnFramebufferSize(const glm::ivec2& size) = 0;
	virtual void OnContentScale(const glm::vec2& size)     = 0;

	void AddInputListener(InputCallbackType callback) {
		if (callback) mInputListeners.emplace_back(callback);
	}
	void AddFramebufferSizeListener(FramebufferSizeCallbackType callback) {
		if (callback) {
			mFramebufferSizeListeners.emplace_back(callback);
			callback(mFramebufferSize);
		}
	}
	void AddContentScaleListener(ContentScaleCallbackType callback) {
		if (callback) {
			mContentScaleListeners.emplace_back(callback);
			callback(mContentScale);
		}
	}

protected:
	GLFWwindow* mWindow;
	glm::ivec2 mFramebufferSize;
	glm::vec2 mContentScale;

private:
	std::vector<InputCallbackType> mInputListeners;
	std::vector<FramebufferSizeCallbackType> mFramebufferSizeListeners;
	std::vector<ContentScaleCallbackType> mContentScaleListeners;
};

}  // namespace ty