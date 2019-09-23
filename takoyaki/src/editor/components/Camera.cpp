#include "Camera.h"

namespace ty {
/*
    Camera(glm::vec3 position = glm::vec3(0.f, 1.f, -5.f),
           glm::vec3 up       = glm::vec3(0.f, 1.f, 0.f),
           float yaw          = 90.f,
           float pitch        = 0.f)
        : mPosition(position)
        , mWorldUp(up)
        , mYaw(yaw)
        , mPitch(pitch)
        , mSpeed(5.0f)
        , mSensitivity(0.25f)
        , mZoom(45.f) {
        */
void Camera::Reset() {
	mIsActive = false;
	std::fill(mKeys.begin(), mKeys.end(), false);

	mPosition    = glm::vec3(0.f, 1.f, -5.f);
	mWorldUp     = glm::vec3(0.f, 1.f, 0.f);
	mYaw         = 90.f;
	mPitch       = 0.f;
	mSpeed       = 5.0f;
	mSensitivity = 0.25f;
	mZoom        = 45.f;
	UpdateCameraVectors();
}
void Camera::Update(float deltaTime) {
	if (mIsActive) {
		UpdateMovement(deltaTime);
	}

	if (mVisibility) {
		if (ImGui::Begin("Camera", &mVisibility)) {
			ImGui::InputFloat3("position", &mPosition.x, 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat3("target", &mTarget.x, 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat3("forward", &mForward.x, 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat3("right", &mRight.x, 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat3("up", &mUp.x, 3, ImGuiInputTextFlags_ReadOnly);
		}
		ImGui::End();
	}
}

void Camera::ProcessKeyInput(const KeyInput& input) {
	mKeys[input.key] = input.action != GLFW_RELEASE;
	mKeyMods         = input.mods;
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

void Camera::UpdateMovement(float deltaTime) {
	float velocity = mSpeed * deltaTime;

	if (mKeyMods & GLFW_MOD_CONTROL) velocity *= 0.5f;
	if (mKeyMods & GLFW_MOD_SHIFT) velocity *= 2.0f;

	if (mKeys[GLFW_KEY_W]) mPosition += mForward * velocity;
	if (mKeys[GLFW_KEY_S]) mPosition -= mForward * velocity;
	if (mKeys[GLFW_KEY_D]) mPosition += mRight * velocity;
	if (mKeys[GLFW_KEY_A]) mPosition -= mRight * velocity;
	if (mKeys[GLFW_KEY_E]) mPosition += mUp * velocity;
	if (mKeys[GLFW_KEY_Q]) mPosition -= mUp * velocity;

	mTarget = mPosition + mForward;
}

}  // namespace ty