#include "MainWindow.h"

namespace {
void SetupImGuiStyle(float scaling) {
	ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 16.0f * scaling);

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors    = style.Colors;

	colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg]              = ImVec4(0.25f, 0.25f, 0.25f, 0.75f);
	colors[ImGuiCol_PopupBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border]                = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
	colors[ImGuiCol_BorderShadow]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg]               = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive]         = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg]               = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive]         = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg]             = ImVec4(0.33f, 0.33f, 0.33f, 0.75f);
	colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_CheckMark]             = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_SliderGrab]            = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_Button]                = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
	colors[ImGuiCol_ButtonHovered]         = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
	colors[ImGuiCol_ButtonActive]          = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_Header]                = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered]         = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_HeaderActive]          = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
	colors[ImGuiCol_Separator]             = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
	colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
	colors[ImGuiCol_SeparatorActive]       = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
	colors[ImGuiCol_ResizeGrip]            = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.86f, 0.66f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
	colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_Tab]                   = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabHovered]            = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_TabActive]             = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused]          = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);

	style.PopupRounding = 3.f;

	style.WindowPadding = glm::vec2(4.f, 4.f);
	style.FramePadding  = glm::vec2(6.f, 4.f);
	style.ItemSpacing   = glm::vec2(6.f, 2.f);

	style.ScrollbarSize = 18.f;

	style.WindowBorderSize = 0.f;
	style.ChildBorderSize  = 0.f;
	style.PopupBorderSize  = 1.f;
	style.FrameBorderSize  = 0.f;

	style.WindowRounding    = 2.f;
	style.ChildRounding     = 2.f;
	style.FrameRounding     = 2.f;
	style.ScrollbarRounding = 2.f;
	style.GrabRounding      = 3.f;

	style.TabBorderSize = 0.f;
	style.TabRounding   = 3.f;

	style.ScaleAllSizes(scaling);
}

static void ErrorCallback(int error, const char* description) {
	std::cout << "Error[" << error << "]: " << description << std::endl;
}

}  // namespace

namespace ty {

MainWindow::MainWindow(int width, int height, const char* title) {
	glfwSetErrorCallback(::ErrorCallback);
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW!");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	mWindow = glfwCreateWindow(width, height, "TakoYaki", NULL, NULL);
	if (!mWindow) {
		throw std::runtime_error("Failed to create a GLFW window!");
	}

	glfwGetFramebufferSize(mWindow, &mFramebufferSize.x, &mFramebufferSize.y);

	InitCallbacks();
	InitGL();
	InitImGui();
}

MainWindow::~MainWindow() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void MainWindow::InitCallbacks() {
	glfwSetWindowUserPointer(mWindow, this);

	glfwSetKeyCallback(mWindow, WindowInputCallback);
	glfwSetFramebufferSizeCallback(mWindow, WindowFramebufferSizeCallback);
}

void MainWindow::InitGL() {
	glfwMakeContextCurrent(mWindow);
	gladLoadGL();
	glfwSwapInterval(1);
}

void MainWindow::InitImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init("#version 450");

	float xS, yS;
	glfwGetWindowContentScale(mWindow, &xS, &yS);
	::SetupImGuiStyle(xS);
}

bool MainWindow::ShouldClose() const {
	return glfwWindowShouldClose(mWindow);
}

void MainWindow::RequestClose() {
	glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
}

void MainWindow::SwapBuffers() {
	glfwSwapBuffers(mWindow);
}

void MainWindow::PollEvents() {
	glfwPollEvents();
}

const glm::ivec2& MainWindow::GetFramebufferSize() const {
	return mFramebufferSize;
}

void MainWindow::OnInput(const KeyInput& input) {
	if (input.key == GLFW_KEY_ESCAPE && input.action == GLFW_PRESS) {
		RequestClose();
	}
}

void MainWindow::OnFramebufferSize(const glm::ivec2& size) {
	mFramebufferSize = size;
}

}  // namespace ty
