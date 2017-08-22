#include "IFractureCamera.h"
#include <gtx/euler_angles.hpp>

glm::vec3 IFractureCamera::UP = glm::vec3(0, 1, 0);

IFractureCamera::IFractureCamera()
{
	m_zNear = 0.1f;
	m_zFar = 1000.0f;
}

IFractureCamera::~IFractureCamera()
{
}

void IFractureCamera::SetProjection(const float fov, const float aspectRatio, const float near, const float far)
{
	m_projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
	m_zNear = near;
	m_zFar = far;
	m_fov = fov;
	m_aspectRatio = aspectRatio;
}

void IFractureCamera::Rotate(const float yaw, const float pitch, const float roll)
{
	m_yaw = glm::radians(yaw);
	m_pitch = glm::radians(pitch);
	m_roll = glm::radians(roll);
	Update();
}

const glm::mat4 IFractureCamera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const glm::mat4 IFractureCamera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

void IFractureCamera::SetPosition(const glm::vec3 pos)
{
	m_position = pos;
}

const glm::vec3 IFractureCamera::GetPosition() const
{
	return m_position;
}

void IFractureCamera::SetFov(const float fovInDegrees)
{
	m_fov = fovInDegrees;
	m_projectionMatrix = glm::perspective(fovInDegrees, m_aspectRatio, m_zNear, m_zFar);
}

const float IFractureCamera::GetFOV() const
{
	return m_fov;
}

const float IFractureCamera::GetAspectRatio() const
{
	return m_aspectRatio;
}

const glm::mat4 IFractureCamera::GetRotationMatrix() const
{
	return m_rotationMatrix;
}