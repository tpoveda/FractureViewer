#define _USE_MATH_DEFINES
#include "FractureTargetCamera.h"
#include <gtx/euler_angles.hpp>
#include <algorithm>

FractureTargetCamera::FractureTargetCamera()
{
	m_right = glm::vec3(1, 0, 0);
	m_up = glm::vec3(0, 1, 0);
	m_look = glm::vec3(0, 0, -1);
	m_minRy = -100;
	m_maxRy = 100;
	m_minDistance = 3;
	m_maxDistance = 100000;
}

FractureTargetCamera::~FractureTargetCamera()
{
}

void FractureTargetCamera::Update()
{
	// calculamos la matriz de rotacion usando la rotacion actual
	glm::mat4 rotationMatrix = glm::yawPitchRoll(m_yaw, m_pitch, 0.0f);

	// creamos un vector y lo trasladamos usando la matriz de rotacion actual
	glm::vec3 translation = glm::vec3(0, 0, m_distance);
	translation = glm::vec3(rotationMatrix * glm::vec4(translation, 0.0f));

	// calculamos la nueva posicion de la camara añadiendo el vector traslacion al punto objetivo
	m_position = m_target + translation;

	// recalculamos la ortonormal de la camara (vectores son perpendiculare entre si y con modulo 1)
	m_look = glm::normalize(m_target - m_position);
	m_up = glm::vec3(rotationMatrix * glm::vec4(UP, 0.0f));
	m_right = glm::cross(m_look, m_up);

	m_rotationMatrix = rotationMatrix;
	m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
}

void FractureTargetCamera::Rotate(const float yaw, const float pitch, const float roll)
{
	float tempPitch = std::min(std::max(pitch, m_minRy), m_maxRy);
	IFractureCamera::Rotate(yaw, tempPitch, roll);
}

void FractureTargetCamera::SetTarget(const glm::vec3 target)
{
	m_target = target;
	m_distance = glm::distance(m_position, m_target);
	m_distance = std::max(m_minDistance, std::min(m_distance, m_maxDistance));
}

const glm::vec3 FractureTargetCamera::GetTarget() const
{
	return m_target;
}

void FractureTargetCamera::Pan(const float dx, const float dy)
{
	glm::vec3 X = m_right * dx;
	glm::vec3 Y = m_up * dy;
	m_position += X + Y;
	m_target += X + Y;
	Update();
}

void FractureTargetCamera::Zoom(const float amount)
{
	m_position += m_look * amount;
	m_distance = glm::distance(m_position, m_target);
	m_distance = std::max(m_minDistance, std::min(m_distance, m_maxDistance));
	Update();
}

void FractureTargetCamera::Move(const float dx, const float dy)
{
	glm::vec3 X = m_right * dx;
	glm::vec3 Y = m_look * dy;
	m_position += X + Y;
	m_target += X + Y;
	Update();
}