#pragma once

namespace ty {

class Camera {
public:
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
		UpdateCameraVectors();
	}

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