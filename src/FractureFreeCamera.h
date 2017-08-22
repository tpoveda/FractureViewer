#ifndef FractureFreeCamera_H
#define FractureFreeCamera_H

#include "IFractureCamera.h"

class FractureFreeCamera : public IFractureCamera
{
public:
	FractureFreeCamera();
	~FractureFreeCamera();

	void Update();

	void Walk(const float dt);
	void Strafe(const float dt);
	void Lift(const float dt);
	void SetTranslation(const glm::vec3& t);

	glm::vec3 GetTranslation() const;
	void SetSpeed(const float speed);
	const float GetSpeed() const;
	
protected:
	//! Velocidad de movimiento de la camara en m/s
	float m_speed;

	//! Traslacion actual de la camara
	glm::vec3 m_translation;
};


#endif