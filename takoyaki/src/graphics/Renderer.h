#pragma once

#include "commands.h"

namespace ty {

class Renderer {
public:
	void NewFrame();
	void ProcessCommands();
	CommandList<Command>& Commands();

private:
	CommandList<Command> mCommandList;
};

}  // namespace ty