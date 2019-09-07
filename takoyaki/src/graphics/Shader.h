#pragma once

#include <glad/glad.h>

namespace ty {

template <GLenum SHADER_TYPE>
class Shader {
public:
	Shader(const char* shaderCode) {
		mShader = glCreateShader(SHADER_TYPE);
		glShaderSource(mShader, 1, &shaderCode, NULL);
		glCompileShader(mShader);

		int success(-1);
		glGetShaderiv(mShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char buffer[512];
			glGetShaderInfoLog(mShader, GL_COMPILE_STATUS, nullptr, buffer);
			throw std::runtime_error(buffer);
		}
	}

	GLuint mShader;
};

using VertexShader = Shader<GL_VERTEX_SHADER>;
using FragmentShader = Shader<GL_FRAGMENT_SHADER>;

}  // namespace ty