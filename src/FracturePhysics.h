#ifndef FracturePhysics_H
#define FracturePhysics_H

class FractureMesh;
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <vector>
#include <iostream>

using namespace std;

enum TiposBounding{ cubo, capsula, capsulax, capsulaz, cilindro, cilindrox, cilindroz, cono, conox, conoz, esfera, malla };

class FracturePhysics
{
public:
	FracturePhysics();
	~FracturePhysics();

	bool Init();
	void Stop();
	void DrawDebug();
	void RemoveRigidBody(btRigidBody *rigid);

	btConvexHullShape* CreateConvexHullShape(FractureMesh *Mesh);
	btRigidBody* createRigid(btScalar TMass, FractureMesh *Mesh, int tipo);
	btDiscreteDynamicsWorld* getWorld();

private:
	btDiscreteDynamicsWorld* m_pWorld;
	btSequentialImpulseConstraintSolver* m_pSolver;
	btBroadphaseInterface *m_pBroadphase;
	btCollisionDispatcher* m_pDispatcher;
	btDefaultCollisionConfiguration* m_pCollisionConfiguration;

	std::vector<btRigidBody*> rigidbodies;
};


#endif