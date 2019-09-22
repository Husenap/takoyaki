#include "DockSpace.h"

namespace ty {

const char* DockSpaceName       = "TakoyakiDockSpace";
const char* DockSpaceWindowName = "TakoyakiDockSpaceWindow";

void DockSpace::Update() {
	ImGuiWindowFlags dockSpaceWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
	                                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
	                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
	                                        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin(DockSpaceWindowName, nullptr, dockSpaceWindowFlags);

	ImGuiID dockSpaceId = ImGui::GetID(DockSpaceName);
	ImGui::DockSpace(dockSpaceId);
	ImGui::End();
	ImGui::PopStyleVar(2);
}

}  // namespace ty