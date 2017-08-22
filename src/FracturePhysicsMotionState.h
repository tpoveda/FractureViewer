#ifndef FracturePhysicsMotionState_H
#define FracturePhysicsMotionState_H

#include "btBulletCollisionCommon.h"

class FracturePhysicsMotionState : public btDefaultMotionState
{
public:
	FracturePhysicsMotionState(const btTransform &transform) : btDefaultMotionState(transform) {}

	void GetWorldTransform(btScalar* transform);
};

#endif