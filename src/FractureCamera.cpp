#define _USE_MATH_DEFINES

#include "FractureCamera.h"
#include <gtx/euler_angles.hpp>
#include <algorithm>

FractureCamera::FractureCamera()
{
	m_cameraType = CameraType::TARGET;

	// Target camera
	m_right = glm::vec3(1, 0, 0);
	m_up = glm::vec3(0, 1, 0);
	m_look = glm::vec3(0, 0, -1);
	m_minRy = -100;
	m_maxRy = 100;
	m_minDistance = 3;
	m_maxDistance = 1000000;

	// Free camera
	m_translation = glm::vec3(0, 0, 0);
	m_speed = 0.5f;		// m/s
}

FractureCamera::~FractureCamera()
{
}

FractureCamera::CameraType FractureCamera::GetCameraType()
{
	return m_cameraType;
}

void FractureCamera::SetCameraType(CameraType cameraType)
{
	m_cameraType = cameraType;
	ResetCamera();
}

void FractureCamera::Update()
{
	if (m_cameraType == CameraType::TARGET)
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
	else if (m_cameraType == CameraType::FREE)
	{
		// calculamos la matriz de rotacion usando la rotacion actual y recalculamos la nueva posicion
		glm::mat4 rotationMatrix = glm::yawPitchRoll(m_yaw, m_pitch, m_roll);
		m_position += m_translation;

		// recalculamos la ortonormal de la camara (vectores son perpendiculare entre si y con modulo 1)
		m_look = glm::vec3(rotationMatrix * glm::vec4(0, 0, 1, 0));
		m_up = glm::vec3(rotationMatrix * glm::vec4(0, 1, 0, 0));
		m_right = glm::cross(m_look, m_up);

		m_rotationMatrix = rotationMatrix;

		glm::vec3 tgt = m_position + m_look;
		m_viewMatrix = glm::lookAt(m_position, tgt, m_up);
	}
}

void FractureCamera::Rotate(const float yaw, const float pitch, const float roll)
{
	float tempPitch = std::min(std::max(pitch, m_minRy), m_maxRy);
	IFractureCamera::Rotate(yaw, tempPitch, roll);
}

void FractureCamera::SetTarget(const glm::vec3& target)
{
	m_target = target;
	m_distance = glm::distance(m_position, m_target);
	m_distance = std::max(m_minDistance, std::min(m_distance, m_maxDistance));
}

const glm::vec3 FractureCamera::GetTarget() const
{
	return m_target;
}

void FractureCamera::Pan(const float dx, const float dy)
{
	glm::vec3 X = m_right * dx;
	glm::vec3 Y = m_up * dy;
	m_position += X + Y;
	m_target += X + Y;
	Update();
}

void FractureCamera::Zoom(const float amount)
{
	m_position += m_look * amount;
	m_distance = glm::distance(m_position, m_target);
	m_distance = std::max(m_minDistance, std::min(m_distance, m_maxDistance));
	Update();
}

void FractureCamera::Move(const float dx, const float dy)
{
	glm::vec3 X = m_right * dx;
	glm::vec3 Y = m_look * dy;
	m_position += X + Y;
	m_target += X + Y;
	Update();
}

void FractureCamera::Walk(const float dt)
{
	m_translation += (m_look * m_speed * dt);
	Update();
}

void FractureCamera::Strafe(const float dt)
{
	m_translation += (m_right * m_speed * dt);
	Update();
}

void FractureCamera::Lift(const float dt)
{
	m_translation += (m_up * m_speed * dt);
	Update();
}

void FractureCamera::SetTranslation(const glm::vec3& t)
{
	m_translation = t;
	Update();
}

const glm::vec3 FractureCamera::GetTranslation() const
{
	return m_translation;
}

void FractureCamera::SetSpeed(const float speed)
{
	m_speed = speed;
}

const float FractureCamera::GetSpeed() const
{
	return m_speed;
}

void FractureCamera::ResetCamera()
{
	m_right = glm::vec3(1, 0, 0);
	m_up = glm::vec3(0, 1, 0);
	m_look = glm::vec3(0, 0, -1);
	m_minRy = -100;
	m_maxRy = 100;
	m_minDistance = 3;
	m_maxDistance = 1000000;

	// Free camera
	m_translation = glm::vec3(0, 0, 0);
	m_speed = 0.5f;		// m/s

	glm::vec3 p = glm::vec3(5);
	SetPosition(p);
	SetProjection(45.0f, GetAspectRatio());
	rX = -45.0f;
	rY = -30.0f;
	dt = 0;
	dst = 10;
	oldX = 0;
	oldY = 0;
	fov = 45.0f;

	if (m_cameraType == CameraType::TARGET)
	{
		SetTarget(glm::vec3(0, 0, 0));
		Rotate(rX, rY, 0);
	}
	else
	{
		glm::vec3 look = glm::normalize(p);
		float yaw = glm::degrees(float(atan2(look.z, look.x) + M_PI));
		float pitch = glm::degrees(asin(look.y));
		rX = yaw;
		rY = pitch;
		Rotate(rX, rY, 0);
	}
}