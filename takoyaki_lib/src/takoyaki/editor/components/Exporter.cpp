#include "Exporter.h"

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

namespace ty {

const char* ExportPopupName = "ExportScene";

void Exporter::Update(RenderTarget& renderTarget) {
	if (mShouldExportData) {
		mShouldExportData        = false;
		const char* filter       = "*.png";
		const char* fileToCreate = tinyfd_saveFileDialog("Export Scene", "", 1, &filter, nullptr);
		if (fileToCreate) {
			const auto pixelData = renderTarget.GetPixels();
			stbi_flip_vertically_on_write(1);
			stbi_write_png(fileToCreate, mSize.x, mSize.y, 4, pixelData.data(), 4 * mSize.x);
		}
		renderTarget.Resize(mOldSize);
	}

	if (mShouldOpen) {
		mShouldOpen = false;
		if (!ImGui::IsPopupOpen(ExportPopupName)) {
			ImGui::OpenPopup(ExportPopupName);
			mSize = renderTarget.GetSize();
		}
	}

	if (ImGui::BeginPopupModal(ExportPopupName, nullptr, ImGuiWindowFlags_NoResize)) {
		ImGui::SetWindowSize({0.f, 0.f});
		ImGui::InputInt2("Resolution", &mSize.x);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.f, 0.f));
		ImGui::Columns(2, nullptr, false);
		if (ImGui::Button("Export", {-FLT_EPSILON, 0.0})) {
			DoExport(renderTarget);
			ImGui::CloseCurrentPopup();
		}
		ImGui::NextColumn();
		if (ImGui::Button("Cancel", {-FLT_EPSILON, 0.0})) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::Columns(1);
		ImGui::PopStyleVar();

		ImGui::EndPopup();
	}
}

void Exporter::Export() {
	mShouldOpen = true;
}

void Exporter::DoExport(RenderTarget& renderTarget) {
	mOldSize = renderTarget.GetSize();
	renderTarget.Resize(mSize);
	mShouldExportData = true;
}

}  // namespace ty