#include "FractureFreeCamera.h"
#include <gtx/euler_angles.hpp>

FractureFreeCamera::FractureFreeCamera()
{
	m_translation = glm::vec3(0, 0, 0);
	m_speed = 0.5f;		// velocidad inicial de 0.5 m/s
}

FractureFreeCamera::~FractureFreeCamera()
{
}

void FractureFreeCamera::Update()
{
	glm::mat4 rotationMatrix = glm::yawPitchRoll(m_yaw, m_pitch, m_roll);
	m_position += m_translation;

	m_look = glm::vec3(rotationMatrix * glm::vec4(0, 0, 1, 0));
	m_up = glm::vec3(rotationMatrix * glm::vec4(0, 1, 0, 0));
	m_right = glm::cross(m_look, m_up);

	m_rotationMatrix = rotationMatrix;

	glm::vec3 tgt = m_position + m_look;
	m_viewMatrix = glm::lookAt(m_position, tgt, m_up);
}

void FractureFreeCamera::Walk(const float dt)
{
	m_translation += (m_look * m_speed * dt);
	Update();
}

void FractureFreeCamera::Strafe(const float dt)
{
	m_translation += (m_right * m_speed * dt);
	Update();
}

void FractureFreeCamera::Lift(const float dt)
{
	m_translation += (m_up * m_speed * dt);
	Update();
}

void FractureFreeCamera::SetTranslation(const glm::vec3& t)
{
	m_translation = t;
	Update();
}

glm::vec3 FractureFreeCamera::GetTranslation() const
{
	return m_translation;
}

void FractureFreeCamera::SetSpeed(const float speed)
{
	m_speed = speed;
}

const float FractureFreeCamera::GetSpeed() const
{
	return m_speed;
}