#include "Takoyaki.h"

#include "../graphics/Shader.h"
#include "../graphics/ShaderProgram.h"
#include "../graphics/gl/RenderCommand.h"

namespace {
ImVec2 vertices[3] = {
    {-1.0f, 3.0f},
    {3.0f, -1.0f},
    {-1.0f, -1.0f},
};

static const char* vertexShaderCode   = R"(
#version 450
in vec2 vPos;
void main(){
	gl_Position = vec4(vPos, 1.0, 1.0);
}
)";
static const char* fragmentShaderCodeBegin = R"(
#version 450
uniform float iTime;
uniform vec2 iResolution;
uniform int iFrame;
)";
static const char* fragmentShaderCodeEnd = R"(
void main(){
	mainImage(gl_FragColor.rgba, gl_FragCoord.xy);
}
)";
}  // namespace

namespace ty {

Takoyaki::Takoyaki()
    : mWindow(1024, 768, "Takoyaki") {
	mWindow.AddInputListener([this](const KeyInput& input) { OnInput(input); });
	mWindow.AddFramebufferSizeListener([this](const glm::ivec2& size) { OnFramebufferSize(size); });

	std::ifstream shaderFile("assets/shaders/main_shader.glsl");
	std::string shaderFileCode((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());

	std::string shaderCode = fragmentShaderCodeBegin;
	shaderCode += shaderFileCode;
	shaderCode += fragmentShaderCodeEnd;

	std::cout << shaderCode << std::endl;

	GLuint vertexArrayName;
	GLint vPosLocation;
	GLint iFrameLocation;
	GLint iTimeLocation;
	GLint iResolutionLocation;

	glGenBuffers(1, &vertexArrayName);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayName);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vertexArrayName);

	ty::VertexShader vShader(vertexShaderCode);
	ty::FragmentShader fShader(shaderCode.c_str());
	ty::ShaderProgram program(vShader, fShader);
	vPosLocation        = program.GetAttributeLocation("vPos");
	iFrameLocation      = program.GetUniformLocation("iTime");
	iTimeLocation       = program.GetUniformLocation("iTime");
	iResolutionLocation = program.GetUniformLocation("iResolution");

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

		cmds.Push<ty::Commands::Viewport>(0, 0, size.x, size.y);
		cmds.Push<ty::Commands::ClearColor>(0.18f, 0.18f, 0.18f, 1.0f);
		cmds.Push<ty::Commands::Clear>(GL_COLOR_BUFFER_BIT);

		cmds.Push<ty::Commands::UseProgram>(program.mProgram);
		cmds.Push<ty::Commands::Uniform>(iFrameLocation, frame);
		cmds.Push<ty::Commands::Uniform>(iTimeLocation, time);
		cmds.Push<ty::Commands::Uniform>(iResolutionLocation, glm::vec2(size.x, size.y));

		cmds.Push<ty::Commands::BindVertexArray>(vertexArrayName);
		cmds.Push<ty::Commands::VertexAttribPointer>(
		    vPosLocation, 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(vertices[0]), nullptr);
		cmds.Push<ty::Commands::EnableVertexAttribArray>(vPosLocation);
		cmds.Push<ty::Commands::DrawArrays>(GL_TRIANGLES, 0, 3);

		mRenderer.ProcessCommands();

		mWindow.SwapBuffers();
	}
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

}  // namespace ty