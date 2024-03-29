#include "Takoyaki.h"

#include "../graphics/MainWindow.h"
#include "../graphics/Renderer.h"
#include "../graphics/gl/RenderCommand.h"
#include "MainEditor.h"
#include "components/Camera.h"
#include "components/UniformsMenu.h"
#include "systems/animator/AnimationSystem.h"
#include "systems/music/MusicSystem.h"

namespace {
ImVec2 vertices[3] = {
    {-1.0f, 3.0f},
    {3.0f, -1.0f},
    {-1.0f, -1.0f},
};

static const char* vertexShaderCode = R"(
#version 450
in vec2 vPos;
void main(){
	gl_Position = vec4(vPos, 1.0, 1.0);
}
)";

static const char* fragmentShaderCodeBegin = R"(
#version 450
out vec4 frag_color;
uniform float iTime;
uniform vec2 iResolution;
uniform int iFrame;
uniform vec3 iCameraOrigin;
uniform vec3 iCameraTarget;
)";

static const char* fragmentShaderCodeEnd = R"(
void main(){
	mainImage(frag_color, gl_FragCoord.xy);
}
)";

static const char* templateShaderCode = R"(// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

void mainImage(out vec4 fragColor, in vec2 fragCoord) { 
    vec2 uv = (fragCoord*2.0 - iResolution.xy) / iResolution.y;

    vec3 col = vec3(0.0);
	col.rg = uv.xy;

    fragColor = vec4(col, 1.0);
}
)";

static const char* glslFileTypeFilter = "*.glsl";
}  // namespace

namespace ty {
Takoyaki::Takoyaki(MainWindow& window,
                   Renderer& renderer,
                   FileWatcher& fileWatcher,
                   MainEditor& editor,
                   Camera& camera,
                   UniformsMenu& uniformsMenu,
                   MusicSystem& musicSystem,
                   AnimationSystem& animationSystem)
    : mWindow(window)
    , mRenderer(renderer)
    , mFileWatcher(fileWatcher)
    , mEditor(editor)
    , mCamera(camera)
    , mUniformsMenu(uniformsMenu)
    , mMusic(musicSystem)
    , mAnimationSystem(animationSystem) {
	SetupListeners();
	CreateVertexBuffer();
	CreateRenderTarget();

	float time      = (float)glfwGetTime();
	float deltaTime = 0.0f;
	float demoTime  = 0.0f;

	const char* filter     = "*.*";
	const char* fileToLoad = tinyfd_openFileDialog("Choose a soundtrack", "", 1, &filter, nullptr, 0);
	if (fileToLoad) {
		if (mMusic.LoadMusic(fileToLoad)) {
			mMusic.Play();
		}
	}

	while (!mWindow.ShouldClose()) {
		static int frame = 0;
		++frame;

		mWindow.PollEvents();
		mRenderer.NewFrame();

		deltaTime = (float)glfwGetTime() - time;
		time      = (float)glfwGetTime();
		mCurrentTime += deltaTime;

		if (mMusic.IsLoaded()) {
			demoTime = mMusic.GetCurrentPosition();
		}

		glm::ivec2 size = mWindow.GetFramebufferSize();

		mEditor.Update(deltaTime, !mCurrentProject.empty(), *mRenderTarget);

		auto& cmds = mRenderer.Commands();
		cmds.Clear();

		cmds.Push<Commands::BindFramebuffer>(0);
		cmds.Push<Commands::Viewport>(0, 0, size.x, size.y);

		cmds.Push<Commands::ClearColor>(0.18f, 0.18f, 0.18f, 1.0f);
		cmds.Push<Commands::Clear>(GL_COLOR_BUFFER_BIT);

		if (!mCurrentProject.empty()) {
			cmds.Push<Commands::BindFramebuffer>(mRenderTarget->GetFramebuffer());
			cmds.Push<Commands::Viewport>(0, 0, mRenderTarget->GetSize().x, mRenderTarget->GetSize().y);

			cmds.Push<Commands::UseProgram>(mProgram->mProgram);
			cmds.Push<Commands::Uniform>(mFrameLoc, frame);
			cmds.Push<Commands::Uniform>(mTimeLoc, demoTime);
			cmds.Push<Commands::Uniform>(mResolutionLoc, glm::vec2(mRenderTarget->GetSize()));
			cmds.Push<Commands::Uniform>(mCameraOriginLoc, mCamera.GetPosition());
			cmds.Push<Commands::Uniform>(mCameraTargetLoc, mCamera.GetTarget());
			mEditor.RegisterCommands(cmds, *mProgram);
			for (const auto& trackValue : mAnimationSystem.Evaluate(demoTime)) {
				cmds.Push<Commands::Uniform>(mProgram->mProgram, trackValue.mName, trackValue.mValue);
			}

			cmds.Push<Commands::BindVertexArray>(mVertexArray);
			cmds.Push<Commands::VertexAttribPointer>(
			    mPosLoc, 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(vertices[0]), nullptr);
			cmds.Push<Commands::EnableVertexAttribArray>(mPosLoc);
			cmds.Push<Commands::DrawArrays>(GL_TRIANGLES, 0, 3);
		}

		cmds.Push<Commands::BindFramebuffer>(0);

		mRenderer.ProcessCommands();

		mWindow.SwapBuffers();

		mFileWatcher.Flush();
	}
}

void Takoyaki::CreateVertexBuffer() {
	glGenBuffers(1, &mVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &mVertexArray);
}

void Takoyaki::CreateRenderTarget() {
	// mRenderTarget = std::make_unique<RenderTarget>(glm::ivec2{1280, 720});
	mRenderTarget = std::make_unique<RenderTarget>(glm::ivec2{2350 / 2, 1000 / 2});
}

void Takoyaki::SetupListeners() {
	mWindow.AddInputListener([this](const KeyInput& input) { OnInput(input); });
	mWindow.AddInputListener([this](const MouseInput& input) { OnInput(input); });
	mWindow.AddInputListener([this](const CursorInput& input) { OnInput(input); });
	mWindow.AddFramebufferSizeListener([this](const glm::ivec2& size) { OnFramebufferSize(size); });
	mWindow.AddContentScaleListener([this](const glm::vec2& scale) { OnContentScale(scale); });

	mEditor.SetNewFileHandler([this]() { OnNewFile(); });
	mEditor.SetOpenFileHandler([this]() { OnOpenFile(); });
	mEditor.SetSaveFileHandler([this]() { OnSaveFile(); });
	mEditor.SetCameraCaptureInputHandler([this]() { OnCameraCaptureInput(); });
	mEditor.SetCameraReleaseInputHandler([this]() { OnCameraReleaseInput(); });

	mUniformsMenu.SetUniformsChangedHandler([this]() { OnUniformsChanged(); });
}

Takoyaki::~Takoyaki() {}

void Takoyaki::OnInput(const KeyInput& input) {
	mEditor.OnInput(input);
}

void Takoyaki::OnInput(const MouseInput& input) {
	mEditor.OnInput(input);
}

void Takoyaki::OnInput(const CursorInput& input) {
	mEditor.OnInput(input);
}

void Takoyaki::OnFramebufferSize(const glm::ivec2& size) {
	mEditor.OnFramebufferSize(size);
}

void Takoyaki::OnContentScale(const glm::vec2& scale) {
	mEditor.OnContentScale(scale);
}

void Takoyaki::LoadShader() {
	mProgram = nullptr;

	std::ifstream shaderFile(mCurrentProject);
	std::string shaderFileCode((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	std::string shaderCode = fragmentShaderCodeBegin;
	shaderCode += mUniformsMenu.GetUniformDeclarations();
	shaderCode += "#line 1\n" + shaderFileCode;
	shaderCode += fragmentShaderCodeEnd;

	VertexShader vShader(vertexShaderCode);
	FragmentShader fShader(shaderCode.c_str());

	mProgram = std::make_unique<ShaderProgram>(vShader, fShader);

	auto error = mProgram->GetError();
	if (error.has_value()) {
		mEditor.ReportError(error.value());
	}

	mPosLoc          = mProgram->GetAttributeLocation("vPos");
	mFrameLoc        = mProgram->GetUniformLocation("iFrame");
	mTimeLoc         = mProgram->GetUniformLocation("iTime");
	mResolutionLoc   = mProgram->GetUniformLocation("iResolution");
	mCameraOriginLoc = mProgram->GetUniformLocation("iCameraOrigin");
	mCameraTargetLoc = mProgram->GetUniformLocation("iCameraTarget");
}

void Takoyaki::OnNewFile() {
	const char* fileToCreate = tinyfd_saveFileDialog("Open TakoYaki file", "", 1, &glslFileTypeFilter, nullptr);
	if (fileToCreate) {
		std::ofstream f(fileToCreate, std::ios_base::trunc);
		if (!f.is_open()) {
			mEditor.ReportError("Failed to create new file!");
			return;
		}

		f << templateShaderCode;
		f.close();
		LoadProjectFile(fileToCreate);
	}
}

void Takoyaki::OnOpenFile() {
	const char* fileToLoad = tinyfd_openFileDialog("Open TakoYaki file", "", 1, &glslFileTypeFilter, nullptr, 0);
	if (fileToLoad) {
		LoadProjectFile(fileToLoad);
	}
}

void Takoyaki::OnSaveFile() {
	if (!mCurrentProject.empty()) {
		mUniformsMenu.SaveFile(mCurrentProject);
	}
}

void Takoyaki::OnCameraCaptureInput() {
	mWindow.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Takoyaki::OnCameraReleaseInput() {
	mWindow.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Takoyaki::LoadProjectFile(const char* fileToLoad) {
	mCurrentTime = 0.0f;

	mCurrentProject = fileToLoad;

	mEditor.LoadProjectFile(mCurrentProject);

	LoadShader();
	mFileWatcher.Clear();
	mFileWatcher.Watch(mCurrentProject, [this](auto&) { LoadShader(); });
}

void Takoyaki::OnUniformsChanged() {
	LoadShader();
}

}  // namespace ty