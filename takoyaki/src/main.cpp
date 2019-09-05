#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <cstring>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main() {
	if (!glfwInit()) {
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	GLFWwindow* window = glfwCreateWindow(800, 600, "TakoYaki", NULL, NULL);
	if (!window) {
		return 1;
	}

	glfwSetKeyCallback(window, KeyCallback);

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSwapInterval(1);

	nk_context* ctx;
	nk_colorf bg{0.18f, 0.18f, 0.18f, 1.0f};
	ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
	{
		nk_font_atlas* atlas;
		nk_glfw3_font_stash_begin(&atlas);
		nk_glfw3_font_stash_end();
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		nk_glfw3_new_frame();

		/* GUI */
		if (nk_begin(ctx,
					 "Demo",
					 nk_rect(50, 50, 230, 250),
					 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE |
						 NK_WINDOW_TITLE)) {
			enum { EASY, HARD };
			static int op = EASY;
			static int property = 20;
			nk_layout_row_static(ctx, 30, 80, 1);
			if (nk_button_label(ctx, "button")) fprintf(stdout, "button pressed\n");

			nk_layout_row_dynamic(ctx, 30, 2);
			if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
			if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;

			nk_layout_row_dynamic(ctx, 25, 1);
			nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

			nk_layout_row_dynamic(ctx, 20, 1);
			nk_label(ctx, "background:", NK_TEXT_LEFT);
			nk_layout_row_dynamic(ctx, 25, 1);
			if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
				nk_layout_row_dynamic(ctx, 120, 1);
				bg = nk_color_picker(ctx, bg, NK_RGBA);
				nk_layout_row_dynamic(ctx, 25, 1);
				bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
				bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
				bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
				bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
				nk_combo_end(ctx);
			}
		}
		nk_end(ctx);

		glDeleteShader(0);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClearColor(bg.r, bg.g, bg.b, bg.a);
		glClear(GL_COLOR_BUFFER_BIT);

		nk_glfw3_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);

		glfwSwapBuffers(window);
	}

	nk_glfw3_shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}