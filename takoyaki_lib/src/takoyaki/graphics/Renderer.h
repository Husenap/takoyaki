#pragma once

#include "gl/RenderCommand.h"

namespace ty {

class Renderer {
public:
	void NewFrame();
	void ProcessCommands();
	RenderCommandList<RenderCommand>& Commands();

private:
	RenderCommandList<RenderCommand> mCommandList;
};

}  // namespace ty