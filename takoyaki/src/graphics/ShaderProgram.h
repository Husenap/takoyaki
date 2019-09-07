#pragma once

#include <type_traits>

namespace ty {

class ShaderProgram {
public:
	template <typename... Shaders>
	ShaderProgram(Shaders... shaders) {
		mProgram = glCreateProgram();
		(AttachShader(shaders), ...);
		glLinkProgram(mProgram);

		int success(-1);
		glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
		if (!success) {
			char buffer[512];
			glGetProgramInfoLog(mProgram, 512, NULL, buffer);
			throw std::runtime_error(buffer);
		}
	}

	GLuint mProgram;

private:
	template<typename T>
	void AttachShader(T&& shader) {
		glAttachShader(mProgram, shader.mShader);
	}
};

}  // namespace ty