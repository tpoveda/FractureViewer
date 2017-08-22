#ifndef IFractureCamera_H
#define IFractureCamera_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class IFractureCamera
{
public:
	IFractureCamera();
	~IFractureCamera();

	void SetProjection(const float fov = 45.0f, const float aspectRatio = 1.3333f, const float near=0.1f, const float far=1000.0f);

	virtual void Update() = 0;
	virtual void Rotate(const float yaw, const float pitch, const float roll);

	const glm::mat4 GetViewMatrix() const;
	const glm::mat4 GetProjectionMatrix() const;
	const glm::mat4 GetRotationMatrix() const;

	void SetPosition(const glm::vec3 pos);
	const glm::vec3 GetPosition() const;

	void SetFov(const float fovInDegrees);
	const float GetFOV() const;
	const float GetAspectRatio() const;

protected:
	float m_yaw;
	float m_pitch;
	float m_roll;
	float m_fov;
	float m_aspectRatio;
	float m_zNear;
	float m_zFar;

	static glm::vec3 UP;

	glm::vec3 m_look;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_position;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_rotationMatrix;
};

#endif