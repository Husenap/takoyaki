#pragma once

#include "Widget.h"

namespace ty {

class Camera : public Widget {
public:
	Camera() {
		Reset();
	}

	void Reset();
	void Update();

	void ProcessKeyInput(bool keys[512], float deltaTime);
	void ProcessMouseMovement(const glm::vec2& delta);

	void CaptureInput();
	void ReleaseInput();

	const glm::vec3& GetPosition() const { return mPosition; }
	const glm::vec3& GetTarget() const { return mTarget; }

private:
	void UpdateCameraVectors();

	glm::vec3 mPosition;
	glm::vec3 mForward;
	glm::vec3 mRight;
	glm::vec3 mUp;
	glm::vec3 mWorldUp;
	glm::vec3 mTarget;
	glm::vec3 mVelocity;
	float mYaw;
	float mPitch;
	float mSpeed;
	float mSensitivity;
	float mZoom;
};

}  // namespace ty