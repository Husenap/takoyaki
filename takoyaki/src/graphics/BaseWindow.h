#pragma once

namespace {
void ErrorCallback(int error, const char* description) {
	std::cout << "Error[" << error << "]: " << description << std::endl;
}
}  // namespace

namespace ty {

template <typename DERIVED_TYPE>
class BaseWindow {
public:
	using KeyInputCallbackType        = std::function<void(const KeyInput&)>;
	using MouseInputCallbackType      = std::function<void(const MouseInput&)>;
	using CursorInputCallbackType     = std::function<void(const CursorInput&)>;
	using FramebufferSizeCallbackType = std::function<void(const glm::ivec2&)>;
	using ContentScaleCallbackType    = std::function<void(const glm::vec2&)>;

public:
	BaseWindow(int width, int height, const char* title) {
		glfwSetErrorCallback(::ErrorCallback);
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW!");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		mWindow = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!mWindow) {
			throw std::runtime_error("Failed to create a GLFW window!");
		}

		glfwSetKeyCallback(mWindow, WindowInputCallback);
		glfwSetFramebufferSizeCallback(mWindow, WindowFramebufferSizeCallback);
		glfwSetWindowContentScaleCallback(mWindow, WindowContentScaleCallback);
		glfwSetCursorPosCallback(mWindow, WindowCursorPosCallback);
		glfwSetMouseButtonCallback(mWindow, WindowMouseButtonCallback);

		glfwGetFramebufferSize(mWindow, &mFramebufferSize.x, &mFramebufferSize.y);
		glfwGetWindowContentScale(mWindow, &mContentScale.x, &mContentScale.y);

		double mx, my;
		glfwGetCursorPos(mWindow, &mx, &my);
		mMousePos   = glm::vec2((float)mx, (float)my);
		mMouseDelta = glm::vec2(0.0f, 0.0f);
	}
	virtual ~BaseWindow() {}

	static void WindowInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto userWindow = reinterpret_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			KeyInput input{key, scancode, action, mods};
			userWindow->OnInput(input);
			for (auto callback : userWindow->mKeyInputListeners) callback(input);
		} else {
			throw std::runtime_error("Failed to process window input callback");
		}
	}

	static void WindowCursorPosCallback(GLFWwindow* window, double x, double y) {
		auto userWindow = reinterpret_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			glm::vec2 newPos((float)x, (float)y);
			userWindow->mMouseDelta = newPos - userWindow->mMousePos;
			userWindow->mMousePos   = newPos;
			CursorInput input;
			input.pos   = userWindow->mMousePos;
			input.delta = userWindow->mMouseDelta;
			userWindow->OnInput(input);
			for (auto callback : userWindow->mCursorInputListeners) callback(input);
		} else {
			throw std::runtime_error("Failed to process window cursor callback");
		}
	}

	static void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		auto userWindow = reinterpret_cast<DERIVED_TYPE*>(glfwGetWindowUserPointer(window));
		if (userWindow) {
			MouseInput input{button, action, mods};
			userWindow->OnInput(input);
			for (auto callback : userWindow->mMouseInputListeners) callback(input);
		} else {
			throw std::runtime_error("Failed to process window mouse button callback");
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
	virtual void OnInput(const MouseInput& mouseInput)     = 0;
	virtual void OnInput(const CursorInput& mouseInput)    = 0;
	virtual void OnFramebufferSize(const glm::ivec2& size) = 0;
	virtual void OnContentScale(const glm::vec2& size)     = 0;

	void AddInputListener(KeyInputCallbackType callback) {
		if (callback) mKeyInputListeners.emplace_back(callback);
	}
	void AddInputListener(MouseInputCallbackType callback) {
		if (callback) mMouseInputListeners.emplace_back(callback);
	}
	void AddInputListener(CursorInputCallbackType callback) {
		if (callback) mCursorInputListeners.emplace_back(callback);
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

	void PollEvents() { glfwPollEvents(); }

	void SwapBuffers() { glfwSwapBuffers(mWindow); }
	void SetInputMode(int mode, int value) { glfwSetInputMode(mWindow, mode, value); }

	void RequestClose() { glfwSetWindowShouldClose(mWindow, GLFW_TRUE); }
	bool ShouldClose() const { return glfwWindowShouldClose(mWindow); }

	const glm::ivec2& GetFramebufferSize() const { return mFramebufferSize; }

protected:
	GLFWwindow* mWindow;
	glm::ivec2 mFramebufferSize;
	glm::vec2 mContentScale;
	glm::vec2 mMousePos;
	glm::vec2 mMouseDelta;

private:
	std::vector<KeyInputCallbackType> mKeyInputListeners;
	std::vector<MouseInputCallbackType> mMouseInputListeners;
	std::vector<CursorInputCallbackType> mCursorInputListeners;
	std::vector<FramebufferSizeCallbackType> mFramebufferSizeListeners;
	std::vector<ContentScaleCallbackType> mContentScaleListeners;
};

}  // namespace ty