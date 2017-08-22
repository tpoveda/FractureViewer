#ifndef FractureTargetCamera_H
#define FractureTargetCamera_H

#include "IFractureCamera.h"

class FractureTargetCamera : public IFractureCamera
{
public:
	FractureTargetCamera();
	~FractureTargetCamera();
	
	//! Actualiza la camara
	void Update() override;

	//! Permite rotar la camara
	//! @param yaw Rotacion en el eje X
	//! @param pitch Rotacion en el eje Y
	//! @param roll Rotacion en el eje Z
	void Rotate(const float yaw, const float pitch, const float roll) override;

	//! Fija el punto objetivo de la camara
	//! @param target Punto objetivo de la camara
	void SetTarget(const glm::vec3 target);
	const glm::vec3 GetTarget() const;


	//! Permite mover tanto la camara como su objetivo en el plano XY
	//! @param dx distancia de traslacion en el eje X
	//! @param dy distancia de traslacion en el eje Y
	void Pan(const float dx, const float dy);

	//! Permite mover tanto la camara como su objetivo en el eje Z
	//! @param AMOUNT distancia de traslacion en el eje z
	void Zoom(const float amount);

	//! Permite mover tanto la camara como su objetivo en los ejes look y right
	//! @param dx distancia de traslacion en el eje look
	//! @param dy distancia de traslacion en el eje right
	void Move(const float dx, const float dy);

protected:
	//! Punto objetivo
	glm::vec3 m_target;

	//! Limite minimo de rotacion en el eje Y
	float m_minRy;

	//! Limite maximo de rotacion en el eje Y
	float m_maxRy;

	//! Ditancia entre la camara y el punto objetivo
	float m_distance;

	//! Limite de distancia minima
	float m_minDistance;

	//! Limite de distancia maxima
	float m_maxDistance;
};


#endif