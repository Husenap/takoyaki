#include "Takoyaki.h"

#include "../graphics/Shader.h"
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
    , mShaderFileToLoad("assets/shaders/main_shader.glsl") {
	mWindow.AddInputListener([this](const KeyInput& input) { OnInput(input); });
	mWindow.AddFramebufferSizeListener([this](const glm::ivec2& size) { OnFramebufferSize(size); });
	mWindow.AddContentScaleListener([this](const glm::vec2& scale) { OnContentScale(scale); });

	GLuint vertexArrayName;

	glGenBuffers(1, &vertexArrayName);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayName);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vertexArrayName);

	ReloadShader();

	int frame = 0;

	while (!mWindow.ShouldClose()) {
		++frame;
		mWindow.PollEvents();
		mRenderer.NewFrame();

		float time      = (float)glfwGetTime();
		glm::ivec2 size = mWindow.GetFramebufferSize();

		mEditor.Update();

		auto& cmds = mRenderer.Commands();
		cmds.Clear();

		cmds.Push<Commands::Viewport>(0, 0, size.x, size.y);
		cmds.Push<Commands::ClearColor>(0.18f, 0.18f, 0.18f, 1.0f);
		cmds.Push<Commands::Clear>(GL_COLOR_BUFFER_BIT);

		cmds.Push<Commands::UseProgram>(mProgram->mProgram);
		cmds.Push<Commands::Uniform>(iFrameLocation, frame);
		cmds.Push<Commands::Uniform>(iTimeLocation, time);
		cmds.Push<Commands::Uniform>(iResolutionLocation, glm::vec2(size.x, size.y));
		mEditor.RegisterCommands(cmds, mProgram);

		cmds.Push<Commands::BindVertexArray>(vertexArrayName);
		cmds.Push<Commands::VertexAttribPointer>(
		    vPosLocation, 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(vertices[0]), nullptr);
		cmds.Push<Commands::EnableVertexAttribArray>(vPosLocation);
		cmds.Push<Commands::DrawArrays>(GL_TRIANGLES, 0, 3);

		mRenderer.ProcessCommands();

		mWindow.SwapBuffers();
	}
}

Takoyaki::~Takoyaki() {}

void Takoyaki::OnInput(const KeyInput& input) {
	mEditor.OnInput(input);

	if (input.key == GLFW_KEY_F2 && input.action == GLFW_PRESS) {
		ReloadShader();
	}
	if (input.key == GLFW_KEY_F3 && input.action == GLFW_PRESS) {
		const char* fileToLoad = tinyfd_openFileDialog("Open TakoYaki file", "", 0, nullptr, nullptr, 0);
		if (fileToLoad) {
			mShaderFileToLoad = fileToLoad;
			ReloadShader();
		}
	}
}

void Takoyaki::OnFramebufferSize(const glm::ivec2& size) {
	mEditor.OnFramebufferSize(size);
}

void Takoyaki::OnContentScale(const glm::vec2& scale) {
	mEditor.OnContentScale(scale);
}

void Takoyaki::ReloadShader() {
	mProgram = nullptr;


	std::ifstream shaderFile(mShaderFileToLoad);
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

	vPosLocation        = mProgram->GetAttributeLocation("vPos");
	iFrameLocation      = mProgram->GetUniformLocation("iFrame");
	iTimeLocation       = mProgram->GetUniformLocation("iTime");
	iResolutionLocation = mProgram->GetUniformLocation("iResolution");
}

}  // namespace ty