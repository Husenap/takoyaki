#include "Camera.h"

namespace ty {

void Camera::Update() {
	if (ImGui::Begin("Camera")) {
		ImGui::InputFloat3("position", &mPosition.x, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("target", &mTarget.x, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("forward", &mForward.x, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("right", &mRight.x, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("up", &mUp.x, 3, ImGuiInputTextFlags_ReadOnly);
	}
	ImGui::End();
}

void Camera::ProcessKeyInput(bool keys[512], float deltaTime) {
	float velocity = mSpeed * deltaTime;

	if (keys[GLFW_MOD_CONTROL]) velocity *= 0.5f;
	if (keys[GLFW_MOD_SHIFT]) velocity *= 2.f;

	if (keys[GLFW_KEY_W]) mPosition += mForward * velocity;
	if (keys[GLFW_KEY_S]) mPosition -= mForward * velocity;
	if (keys[GLFW_KEY_D]) mPosition += mRight * velocity;
	if (keys[GLFW_KEY_A]) mPosition -= mRight * velocity;
	if (keys[GLFW_KEY_E]) mPosition += mUp * velocity;
	if (keys[GLFW_KEY_Q]) mPosition -= mUp * velocity;

	mTarget = mPosition + mForward;
}

void Camera::ProcessMouseMovement(const glm::vec2& delta) {
	mYaw -= delta.x * mSensitivity;
	mPitch -= delta.y * mSensitivity;

	mPitch = glm::clamp(mPitch, -89.9f, 89.9f);

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
	glm::vec3 forward(std::cosf(glm::radians(mYaw)) * std::cosf(glm::radians(mPitch)),
	                  std::sinf(glm::radians(mPitch)),
	                  std::sinf(glm::radians(mYaw)) * std::cosf(glm::radians(mPitch)));
	mForward = glm::normalize(forward);
	mRight   = glm::normalize(glm::cross(mWorldUp, mForward));
	mUp      = glm::normalize(glm::cross(mForward, mRight));
	mTarget  = mPosition + mForward;
}

void Camera::CaptureInput() {}

void Camera::ReleaseInput() {}

}  // namespace ty