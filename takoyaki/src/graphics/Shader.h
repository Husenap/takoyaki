#pragma once

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
			try {
				throw std::runtime_error(buffer);
			} catch (const std::runtime_error& e) {
				std::cout << e.what() << std::endl;
			}
		}
	}
	~Shader() { glDeleteShader(mShader); }

	GLuint mShader;
};

using VertexShader   = Shader<GL_VERTEX_SHADER>;
using FragmentShader = Shader<GL_FRAGMENT_SHADER>;

}  // namespace ty