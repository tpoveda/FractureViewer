#include "FracturePhysics.h"
#include "FractureMesh.h"

#include <iostream>

using namespace std;


FracturePhysics::FracturePhysics()
{

}

FracturePhysics::~FracturePhysics()
{

}

bool FracturePhysics::Init()
{
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();

	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);

	m_pBroadphase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));

	m_pSolver = new btSequentialImpulseConstraintSolver();

	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);

	m_pWorld->setGravity(btVector3(0, -9.81f, 0));

	return true;
}

btRigidBody* FracturePhysics::createRigid(btScalar TMass, FractureMesh *Mesh, int tipo)
{
	GLfloat *points = Mesh->GetMesh()->getPointsBounding();	
	
	btCollisionShape *Shape = NULL;

	double ancho = abs(points[1] - points[0])*0.5;
	double alto = abs(points[3] - points[2])*0.5;
	double largo = abs(points[5] - points[4])*0.5;

	switch (tipo)
	{
		case TiposBounding::cubo:
		{
			btVector3 HalfExtents(ancho, alto, largo);
			Shape = new btBoxShape(HalfExtents);
			break;
		}

		case TiposBounding::capsula:
		{	
			double distancia = ancho;

			if (distancia < largo)
			{
				distancia = largo;
			}
			
			Shape = new btCapsuleShape(distancia, alto);
			break;
		}

		case TiposBounding::capsulax:
		{
			double distancia = alto;

			if (distancia < largo)
			{
				distancia = largo;
			}			
			
			Shape = new btCapsuleShapeX(distancia, ancho);
			break;
		}

		case TiposBounding::capsulaz:
		{
			double distancia = ancho;

			if (distancia < alto)
			{
				distancia = alto;
			}
		
			Shape = new btCapsuleShapeZ(distancia, largo);
			break;
		}
		case TiposBounding::cono:
		{
			double distancia = ancho;

			if (distancia < largo)
			{
				distancia = largo;
			}

			Shape = new btConeShape(distancia, alto * 2);
			break;
		}

		case TiposBounding::conox:
		{
			double distancia = alto;

			if (distancia < largo)
			{
				distancia = largo;
			}
						
			Shape = new btConeShapeX(distancia, ancho * 2);
			break;
		}

		case TiposBounding::conoz:
		{
			double distancia = ancho;

			if (distancia < alto)
			{
				distancia = alto;
			}
			
			Shape = new btConeShapeZ(distancia, largo * 2);
			break;
		}

		case TiposBounding::cilindro:
		{
			double distancia = ancho;

			if (distancia < largo)
			{
				distancia = largo;
			}

			btVector3 HalfExtents(distancia, alto, distancia);
			Shape = new btCylinderShape(HalfExtents);
			break;
		}
		case TiposBounding::cilindrox:
		{
			double distancia = alto;

			if (distancia < largo)
			{
				distancia = largo;
			}
		
			btVector3 HalfExtents(ancho, distancia, distancia);
			Shape = new btCylinderShapeX(HalfExtents);
			break;
		}

		case TiposBounding::cilindroz:
		{
			double distancia = alto;

			if (distancia < ancho)
			{
				distancia = ancho;
			}

			btVector3 HalfExtents(distancia, distancia, largo);
			Shape = new btCylinderShapeZ(HalfExtents);
			break;
		}

		case TiposBounding::esfera:
		{
			double distancia = ancho;

			if (distancia < largo)
			{
				distancia = largo;
			}

			if (distancia < alto)
			{
				distancia = alto;
			}

			Shape = new btSphereShape(distancia);
			break;
			
		}

		case TiposBounding::malla:
		{
			Shape = Mesh->GetMesh()->getHullShape();
			break;
		}

	}

	btVector3 LocalInertia;
	Shape->calculateLocalInertia(TMass, LocalInertia);

	btRigidBody *RigidBody = new btRigidBody(TMass, 0, Shape, LocalInertia);

	RigidBody->setUserPointer((void *)Mesh);
	RigidBody->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	Mesh->SetRigidBody(RigidBody);

	rigidbodies.push_back(RigidBody);

	m_pWorld->addRigidBody(RigidBody);

	return RigidBody;
}

btDiscreteDynamicsWorld* FracturePhysics::getWorld()
{
	return m_pWorld;
}

void FracturePhysics::RemoveRigidBody(btRigidBody *rigid)
{
	m_pWorld->removeRigidBody(rigid);
}

void FracturePhysics::DrawDebug()
{
	m_pWorld->debugDrawWorld();
}

void FracturePhysics::Stop()
{
	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
	rigidbodies.clear();
}

