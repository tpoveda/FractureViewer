#ifndef FractureCamera_H
#define FractureCamera_H

#include "IFractureCamera.h"

class FractureCamera : public IFractureCamera
{
public:

	enum CameraType
	{
		TARGET,
		FREE
	};

	FractureCamera();
	~FractureCamera();

	void Update() override;
	CameraType GetCameraType();
	void SetCameraType(CameraType cameraType);

	// Target camera
	void Rotate(const float yaw, const float pitch, const float roll) override;
	void SetTarget(const glm::vec3& target);
	const glm::vec3 GetTarget() const;
	void Pan(const float dx, const float dy);
	void Zoom(const float amount);
	void Move(const float dx, const float dy);

	// Free camera
	void Walk(const float dt);
	void Strafe(const float dt);
	void Lift(const float dt);
	void SetTranslation(const glm::vec3& t);
	const glm::vec3 GetTranslation() const;
	void SetSpeed(const float speed);
	const float GetSpeed() const;

	// Variables camara FractureTargetCamera
	float dt = 0;
	float rX = -45;
	float rY = -30;
	float dst = 10;
	float oldX = 0;
	float oldY = 0;
	float fov = 45.0;

protected:
	CameraType m_cameraType;

	// Target camera
	glm::vec3 m_target;
	float m_minRy;
	float m_maxRy;
	float m_distance;
	float m_minDistance;
	float m_maxDistance;

	// Free camera
	float m_speed;
	glm::vec3 m_translation;

private:
	void ResetCamera();
};

#endif