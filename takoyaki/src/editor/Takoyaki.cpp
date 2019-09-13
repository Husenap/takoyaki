#include "Takoyaki.h"

#include "../graphics/gl/RenderCommand.h"

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
)";

static const char* fragmentShaderCodeEnd = R"(
void main(){
	mainImage(frag_color, gl_FragCoord.xy);
}
)";
}  // namespace

namespace ty {

Takoyaki::Takoyaki()
    : mWindow(1024, 768, "Takoyaki")
    , mCurrentProject("assets/shaders/main_shader.glsl") {
	SetupListeners();
	CreateVertexBuffer();
	CreateRenderTarget();
	CreateCopyProgram();

	mFileWatcher.Watch(mCurrentProject, [this](auto&) { LoadShader(); });

	LoadShader();

	mFileWatcher.StartThread();

	while (!mWindow.ShouldClose()) {
		static int frame = 0;
		++frame;

		mWindow.PollEvents();
		mRenderer.NewFrame();

		float time      = (float)glfwGetTime();
		glm::ivec2 size = mWindow.GetFramebufferSize();

		mEditor.Update();

		auto& cmds = mRenderer.Commands();
		cmds.Clear();

		cmds.Push<Commands::BindFramebuffer>(0);
		cmds.Push<Commands::Viewport>(0, 0, size.x, size.y);

		cmds.Push<Commands::ClearColor>(0.18f, 0.18f, 0.18f, 1.0f);
		cmds.Push<Commands::Clear>(GL_COLOR_BUFFER_BIT);

		cmds.Push<Commands::BindFramebuffer>(mRenderTarget->GetFramebuffer());
		cmds.Push<Commands::Viewport>(0, 0, mRenderTarget->GetSize().x, mRenderTarget->GetSize().y);

		cmds.Push<Commands::UseProgram>(mProgram->mProgram);
		cmds.Push<Commands::Uniform>(mFrameLoc, frame);
		cmds.Push<Commands::Uniform>(mTimeLoc, time);
		cmds.Push<Commands::Uniform>(mResolutionLoc, glm::vec2(mRenderTarget->GetSize()));
		mEditor.RegisterCommands(cmds, mProgram);

		cmds.Push<Commands::BindVertexArray>(mVertexArray);
		cmds.Push<Commands::VertexAttribPointer>(mPosLoc, 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(vertices[0]), nullptr);
		cmds.Push<Commands::EnableVertexAttribArray>(mPosLoc);
		cmds.Push<Commands::DrawArrays>(GL_TRIANGLES, 0, 3);

		cmds.Push<Commands::BindFramebuffer>(0);
		cmds.Push<Commands::Viewport>(0, 0, size.x, size.y);

		ImGui::Image((void*)(intptr_t)mRenderTarget->GetRenderTexture(), mRenderTarget->GetSize(), {0, 1}, {1, 0});

		mRenderer.ProcessCommands();

		mWindow.SwapBuffers();

		mFileWatcher.Flush();
	}
	mFileWatcher.StopThread();
}

void Takoyaki::CreateVertexBuffer() {
	glGenBuffers(1, &mVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &mVertexArray);
}

void Takoyaki::CreateRenderTarget() {
	mRenderTarget = std::make_unique<RenderTarget>(mWindow.GetFramebufferSize());
}

void Takoyaki::SetupListeners() {
	mWindow.AddInputListener([this](const KeyInput& input) { OnInput(input); });
	mWindow.AddFramebufferSizeListener([this](const glm::ivec2& size) { OnFramebufferSize(size); });
	mWindow.AddContentScaleListener([this](const glm::vec2& scale) { OnContentScale(scale); });

	mEditor.SetOpenFileHandler([this]() { OnOpenFile(); });
	mEditor.SetSaveFileHandler([this]() { OnSaveFile(); });
}

Takoyaki::~Takoyaki() {}

void Takoyaki::OnInput(const KeyInput& input) {
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
	shaderCode += mEditor.GetUniformsMenu().GetUniformDeclarations();
	shaderCode += shaderFileCode;
	shaderCode += fragmentShaderCodeEnd;

	VertexShader vShader(vertexShaderCode);
	FragmentShader fShader(shaderCode.c_str());

	mProgram = std::make_unique<ShaderProgram>(vShader, fShader);

	auto error = mProgram->GetError();
	if (error.has_value()) {
		mEditor.ReportError(error.value());
	}

	mPosLoc        = mProgram->GetAttributeLocation("vPos");
	mFrameLoc      = mProgram->GetUniformLocation("iFrame");
	mTimeLoc       = mProgram->GetUniformLocation("iTime");
	mResolutionLoc = mProgram->GetUniformLocation("iResolution");
}

void Takoyaki::CreateCopyProgram() {}

void Takoyaki::OnOpenFile() {
	const char* filter     = "*.glsl";
	const char* fileToLoad = tinyfd_openFileDialog("Open TakoYaki file", "", 1, &filter, nullptr, 0);
	if (fileToLoad) {
		mCurrentProject = fileToLoad;
		mEditor.OpenFile(mCurrentProject);
		LoadShader();
	}
}

void Takoyaki::OnSaveFile() {
	mEditor.SaveFile(mCurrentProject);
}

}  // namespace ty