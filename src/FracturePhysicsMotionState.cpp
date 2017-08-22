#include "FracturePhysicsMotionState.h"

void FracturePhysicsMotionState::GetWorldTransform(btScalar* transform)
{
	btTransform trans;
	getWorldTransform(trans);
	trans.getOpenGLMatrix(transform);
}