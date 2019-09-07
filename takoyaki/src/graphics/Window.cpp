#include "Window.h"

namespace {
void WindowInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto userWindow = static_cast<ty::Window*>(glfwGetWindowUserPointer(window));
	if (userWindow) {
		userWindow->InputCallback(key, scancode, action, mods);
	} else {
		throw std::runtime_error("Failed to process window input");
	}
}
}  // namespace

namespace ty {

Window::Window(int width, int height, const char* title) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	mWindow = glfwCreateWindow(800, 600, "TakoYaki", NULL, NULL);
	if (!mWindow) {
		throw std::runtime_error("Failed to create a GLFW window!");
	}

	glfwSetWindowUserPointer(mWindow, this);

	glfwSetKeyCallback(mWindow, ::WindowInputCallback);

	glfwMakeContextCurrent(mWindow);
	gladLoadGL();
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

Window::~Window() {
	glfwDestroyWindow(mWindow);
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(mWindow);
}

void Window::RequestClose() {
	glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
}

void Window::InputCallback(int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		RequestClose();
	}
}

void Window::SwapBuffers() {
	glfwSwapBuffers(mWindow);
}

void Window::PollEvents() {
	glfwPollEvents();
}

}  // namespace ty
