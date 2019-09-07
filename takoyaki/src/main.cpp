#include "graphics/Renderer.h"
#include "graphics/Window.h"

#include "graphics/Commands.h"
#include "graphics/Shader.h"
#include "graphics/ShaderProgram.h"

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

	bool showDemoWindow = false;

	while (!window.ShouldClose()) {
		window.PollEvents();

		double time = glfwGetTime();

		renderer.NewFrame();

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

	return 0;
}