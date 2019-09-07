#include "graphics/Renderer.h"
#include "graphics/Window.h"

#include "graphics/Commands.h"
#include "graphics/Shader.h"
#include "graphics/ShaderProgram.h"

void SetupImGuiStyle() {
	ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 16.0f);

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors    = style.Colors;

	colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg]              = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_PopupBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border]                = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
	colors[ImGuiCol_BorderShadow]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg]               = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive]         = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg]               = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive]         = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg]             = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
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

	style.WindowPadding = ImVec2(4.f, 4.f);
	style.FramePadding  = ImVec2(6.f, 4.f);
	style.ItemSpacing   = ImVec2(6.f, 2.f);

	style.ScrollbarSize = 18.f;

	style.WindowBorderSize = 1.f;
	style.ChildBorderSize  = 1.f;
	style.PopupBorderSize  = 1.f;
	style.FrameBorderSize  = 0.f;

	style.WindowRounding    = 3.f;
	style.ChildRounding     = 3.f;
	style.FrameRounding     = 3.f;
	style.ScrollbarRounding = 2.f;
	style.GrabRounding      = 3.f;

	style.TabBorderSize = 0.f;
	style.TabRounding   = 3.f;
}

static void ErrorCallback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

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

static const char* fragmentShaderCode = R"(
#version 450

uniform float iTime;
uniform vec2 iResolution;

#define SMOOTHNESS (0.002 / (myResolution.y/1080.))
vec2 myResolution;

const vec3 BLACK	= (vec3(0.)/255.0);
const vec3 WHITE	= (vec3(255.)/255.0);
const vec3 RED		= (vec3(205., 46., 58.)/255.0);
const vec3 BLUE		= (vec3(0., 71., 160.)/255.0);

const vec2 NW		= normalize(vec2(-3.0, 2.0));

float cross2(vec2 u, vec2 v){
    return smoothstep(SMOOTHNESS, -SMOOTHNESS, u.x*v.y-u.y*v.x);
}

float dCircle(vec2 p, float r){
    return smoothstep(SMOOTHNESS, -SMOOTHNESS, length(p) - r*0.5);
}
float dBox(vec2 p, vec2 s){
    return smoothstep(SMOOTHNESS, -SMOOTHNESS, max(abs(p.x)-s.x*0.5, abs(p.y)-s.y*0.5));
}
float dStripe(vec2 p, float i, float a, float g){
    float c = cos(a);
    float s = sin(a);
    mat2 m2 = mat2(c, -s, s, c);
    vec2 q = p * m2;
    
    q.y -= 0.75+1./24.;
    q.y -= (3./24.)*i;
    
    return dBox(q, vec2(1./2., 1./12.)) - dBox(q, vec2(1./24., 1./12.)) * g;
}

vec3 taegeukgi(vec2 p){
    vec3 col = WHITE;
    
    col = mix(col, mix(BLUE, RED, vec3(cross2(NW, p))), dCircle(p, 1.0));
    
    col = mix(col, RED, dCircle(p-NW*0.25, 0.5));
    col = mix(col, BLUE, dCircle(p+NW*0.25, 0.5));
    
    col = mix(col, BLACK, dStripe(p, 0., atan(NW.x, NW.y), 0.));
    col = mix(col, BLACK, dStripe(p, 1., atan(NW.x, NW.y), 0.));
    col = mix(col, BLACK, dStripe(p, 2., atan(NW.x, NW.y), 0.));
    
    col = mix(col, BLACK, dStripe(p, 0., atan(NW.x, -NW.y), 0.));
    col = mix(col, BLACK, dStripe(p, 1., atan(NW.x, -NW.y), 1.));
    col = mix(col, BLACK, dStripe(p, 2., atan(NW.x, -NW.y), 0.));
    
    col = mix(col, BLACK, dStripe(p, 0., atan(-NW.x, NW.y), 1.));
    col = mix(col, BLACK, dStripe(p, 1., atan(-NW.x, NW.y), 0.));
    col = mix(col, BLACK, dStripe(p, 2., atan(-NW.x, NW.y), 1.));
    
    col = mix(col, BLACK, dStripe(p, 0., atan(-NW.x, -NW.y), 1.));
    col = mix(col, BLACK, dStripe(p, 1., atan(-NW.x, -NW.y), 1.));
    col = mix(col, BLACK, dStripe(p, 2., atan(-NW.x, -NW.y), 1.));
    
    return col;
}



void mainImage(out vec4 fragColor, in vec2 fragCoord){
	myResolution = iResolution;
	vec2 uv = (fragCoord*2.f - iResolution.xy) / iResolution.y;

	vec3 col = vec3(0.0);

	col = taegeukgi(uv + cos(vec2(iTime)));

	fragColor = vec4(col, 1.0);
}

void main(){
	mainImage(gl_FragColor.rgba, gl_FragCoord.xy);
}
)";

int main() {
	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit()) {
		return 1;
	}

	ty::Renderer renderer;
	ty::Window window(1024, 768, "TakoYaki");

	GLuint vertexArrayName;
	GLint vPosLocation;
	GLint iTimeLocation;
	GLint iResolutionLocation;

	glGenBuffers(1, &vertexArrayName);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayName);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArrayName);

	ty::VertexShader vShader(vertexShaderCode);
	ty::FragmentShader fShader(fragmentShaderCode);
	ty::ShaderProgram program(vShader, fShader);

	vPosLocation        = program.GetAttributeLocation("vPos");
	iTimeLocation       = program.GetUniformLocation("iTime");
	iResolutionLocation = program.GetUniformLocation("iResolution");

	SetupImGuiStyle();

	bool showDemoWindow = false;

	while (!window.ShouldClose()) {
		window.PollEvents();

		double time = glfwGetTime();

		renderer.NewFrame();

		glfwMakeContextCurrent(window.GetHandle());

		if (showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);

		ImGui::Render();

		int width, height;
		glfwGetFramebufferSize(window.GetHandle(), &width, &height);

		auto& cmds = renderer.Commands();
		cmds.Clear();

		cmds.Push<ty::Commands::Viewport>(0, 0, width, height);
		cmds.Push<ty::Commands::ClearColor>(0.18f, 0.18f, 0.18f, 1.0f);
		cmds.Push<ty::Commands::Clear>(GL_COLOR_BUFFER_BIT);

		cmds.Push<ty::Commands::UseProgram>(program.mProgram);
		glUniform1f(iTimeLocation, (float)time);
		glUniform2f(iResolutionLocation, (float)width, (float)height);

		cmds.Push<ty::Commands::BindVertexArray>(vertexArrayName);
		cmds.Push<ty::Commands::VertexAttribPointer>(
		    vPosLocation, 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(vertices[0]), nullptr);
		cmds.Push<ty::Commands::EnableVertexAttribArray>(vPosLocation);
		cmds.Push<ty::Commands::DrawArrays>(GL_TRIANGLES, 0, 3);

		renderer.ProcessCommands();

		window.SwapBuffers();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}