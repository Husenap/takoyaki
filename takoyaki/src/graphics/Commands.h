#pragma once

namespace ty::Commands::Locations {

class UniformLocation {
	const GLint mProgram;
	const std::string mName;

public:
	UniformLocation(GLint program, std::string_view name)
	    : mProgram(program)
	    , mName(name) {
	}

	GLint Get() const {
		return glGetUniformLocation(mProgram, mName.c_str());
	}
};

class AttributeLocation {
	const GLint mProgram;
	const std::string mName;

public:
	AttributeLocation(GLint program, std::string_view name)
	    : mProgram(program)
	    , mName(name) {
	}

	GLint Get() const {
		return glGetAttribLocation(mProgram, mName.c_str());
	}
};

class FixedLocation {
	const GLint mLocation;

public:
	FixedLocation(GLint location)
	    : mLocation(location) {
	}

	GLint Get() const {
		return mLocation;
	}
};

struct GetLocation {
	template <typename T>
	GLint operator()(const T& location) {
		return location.Get();
	}
};

}  // namespace ty::Commands::Locations

namespace ty::Commands {

class Viewport {
	const GLint mX;
	const GLint mY;
	const GLsizei mW;
	const GLsizei mH;

public:
	Viewport(GLint x, GLint y, GLsizei w, GLsizei h)
	    : mX(x)
	    , mY(y)
	    , mW(w)
	    , mH(h) {
	}

	void Apply() {
		glViewport(mX, mY, mW, mH);
	}
};

class ClearColor {
	const GLfloat mR;
	const GLfloat mG;
	const GLfloat mB;
	const GLfloat mA;

public:
	ClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	    : mR(r)
	    , mG(g)
	    , mB(b)
	    , mA(a) {
	}

	void Apply() {
		glClearColor(mR, mG, mB, mA);
	}
};

class Clear {
	const GLbitfield mMask;

public:
	Clear(GLbitfield mask)
	    : mMask(mask) {
	}

	void Apply() {
		glClear(mMask);
	}
};

class UseProgram {
	const GLuint mProgram;

public:
	UseProgram(GLuint program)
	    : mProgram(program) {
	}

	void Apply() {
		glUseProgram(mProgram);
	}
};

class BindVertexArray {
	const GLuint mVertexArrayName;

public:
	BindVertexArray(GLuint vertexArrayName)
	    : mVertexArrayName(vertexArrayName) {
	}

	void Apply() {
		glBindVertexArray(mVertexArrayName);
	}
};

class VertexAttribPointer {
	using LocationHolder = std::variant<Locations::FixedLocation, Locations::AttributeLocation>;

	const LocationHolder mLocation;
	const GLint mSize;
	const GLenum mType;
	const GLboolean mNormalized;
	const GLsizei mStride;
	const GLvoid* mPointer;

public:
	VertexAttribPointer(GLint location,
	                    GLint size,
	                    GLenum type,
	                    GLboolean normalized,
	                    GLsizei stride,
	                    const GLvoid* pointer = nullptr)
	    : mLocation(Locations::FixedLocation(location))
	    , mSize(size)
	    , mType(type)
	    , mNormalized(normalized)
	    , mStride(stride)
	    , mPointer(pointer) {
	}
	VertexAttribPointer(GLint program,
	                    std::string_view name,
	                    GLint size,
	                    GLenum type,
	                    GLboolean normalized,
	                    GLsizei stride,
	                    const GLvoid* pointer = nullptr)
	    : mLocation(Locations::AttributeLocation(program, name))
	    , mSize(size)
	    , mType(type)
	    , mNormalized(normalized)
	    , mStride(stride)
	    , mPointer(pointer) {
	}

	void Apply() {
		glVertexAttribPointer(
		    std::visit(Locations::GetLocation(), mLocation), mSize, mType, mNormalized, mStride, mPointer);
	}
};

class EnableVertexAttribArray {
	using LocationHolder = std::variant<Locations::FixedLocation, Locations::AttributeLocation>;

	const LocationHolder mLocation;

public:
	EnableVertexAttribArray(GLint location)
	    : mLocation(Locations::FixedLocation(location)) {
	}
	EnableVertexAttribArray(GLint program, std::string_view name)
	    : mLocation(Locations::AttributeLocation(program, name)) {
	}

	void Apply() {
		glEnableVertexAttribArray(std::visit(Locations::GetLocation(), mLocation));
	}
};

class DrawArrays {
	GLenum mMode;
	GLint mFirst;
	GLsizei mCount;

public:
	DrawArrays(GLenum mode, GLint first, GLsizei count)
	    : mMode(mode)
	    , mFirst(first)
	    , mCount(count) {
	}

	void Apply() {
		glDrawArrays(mMode, mFirst, mCount);
	}
};

// glEnableVertexAttribArray(vPosLocation);
// glUniform1f(iTimeLocation, (float)time);
// glUniform2f(iResolutionLocation, (float)width, (float)height);
// glDrawArrays(GL_TRIANGLES, 0, 3);

}  // namespace ty::Commands

namespace ty {

using Command = std::variant<std::monostate,
                             Commands::Viewport,
                             Commands::ClearColor,
                             Commands::Clear,
                             Commands::UseProgram,
                             Commands::BindVertexArray,
                             Commands::VertexAttribPointer,
                             Commands::EnableVertexAttribArray,
                             Commands::DrawArrays>;

template <typename Command>
struct ImmediateCommand {
	template <typename... Args>
	ImmediateCommand(Args&&... args) {
		Command cmd(std::forward<Args>(args)...);
		cmd.Apply();
	}
};

struct CommandRunner {
	template <typename T>
	void operator()(T& command) const {
		command.Apply();
	}

	void operator()(std::monostate&) const {
		throw std::runtime_error("Should there be a monostate in the commandlist?");
	}
};

template <typename T>
class CommandList {
public:
	template <typename CommandType, typename... Args>
	void Push(Args&&... args) {
		mCommandList.emplace_back(std::in_place_type_t<CommandType>{}, std::forward<Args>(args)...);
	}

	void Clear() {
		mCommandList.clear();
	}

	template <typename Visitor>
	void Visit(Visitor&& visitor) {
		for (auto& command : mCommandList) {
			std::visit(visitor, command);
		}
	}

private:
	std::vector<T> mCommandList;
};
}  // namespace ty