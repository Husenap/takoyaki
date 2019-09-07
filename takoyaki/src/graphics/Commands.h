#pragma once

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
	GLfloat mR;
	GLfloat mG;
	GLfloat mB;
	GLfloat mA;

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
	GLbitfield mMask;

public:
	Clear(GLbitfield mask)
	    : mMask(mask) {
	}

	void Apply() {
		glClear(mMask);
	}
};

using RenderCommand = std::variant<std::monostate, Viewport, ClearColor, Clear>;

template<typename Command>
struct ImmediateCommand {
	template<typename ...Args>
	ImmediateCommand(Args&&... args) {
		Command cmd(std::forward<Args>(args)...);
		cmd.Apply();
	}
};

struct CommandRunner {
	template<typename T>
	void operator()(T& command) const {
		command.Apply();
	}
};

template <typename T>
class CommandList {
public:
	template<typename Visitor>
	void Visit(Visitor&& visitor) const {
		for (auto& command : mCommandList) {
			std::visit(visitor, command);
		}
	}

private:
	std::vector<T> mCommandList;
};
}  // namespace ty::Commands