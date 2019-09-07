#include "Renderer.h"

namespace ty {

void Renderer::NewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Renderer::ProcessCommands() {
	mCommandList.Visit(CommandRunner());

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ty::CommandList<ty::Command>& Renderer::Commands() {
	return mCommandList;
}

}  // namespace ty