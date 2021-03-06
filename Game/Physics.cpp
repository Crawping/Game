//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

namespace Physics
{

	btDefaultCollisionConfiguration *		CollisionConfiguration;
	btCollisionDispatcher *					Dispatcher;
	btBroadphaseInterface *					OverlappingPairCache;
	btSequentialImpulseConstraintSolver *	Solver;
	btDiscreteDynamicsWorld *				DynamicsWorld;

	//////////////////////////////////////////////////////////////////////

	void Physics::Open()
	{
		CollisionConfiguration = new btDefaultCollisionConfiguration();
		Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
		OverlappingPairCache = new btDbvtBroadphase();
		Solver = new btSequentialImpulseConstraintSolver;
		DynamicsWorld = new btDiscreteDynamicsWorld(Dispatcher, OverlappingPairCache, Solver, CollisionConfiguration);
		DynamicsWorld->setGravity(btVector3(0,0,-6));
	}

	//////////////////////////////////////////////////////////////////////

	void Physics::Close()
	{
		// brute force cleanup
		if(DynamicsWorld->getNumCollisionObjects() > 0)
		{
			TRACE("Deleting %d orphaned physics objects\n", DynamicsWorld->getNumCollisionObjects());

			for (int i = DynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
			{
				btCollisionObject* obj = DynamicsWorld->getCollisionObjectArray()[i];
				btRigidBody* body = btRigidBody::upcast(obj);
				if (body && body->getMotionState())
				{
					delete body->getMotionState();
				}
				DynamicsWorld->removeCollisionObject( obj );
				delete obj;
			}
		}

		//delete dynamics world
		delete DynamicsWorld;

		//delete solver
		delete Solver;

		//delete broadphase
		delete OverlappingPairCache;

		//delete dispatcher
		delete Dispatcher;

		delete CollisionConfiguration;
	}

	//////////////////////////////////////////////////////////////////////

	void DeleteRigidBody(btRigidBody * &b)
	{
		if(b != null)
		{
			delete b->getMotionState();
			Physics::DynamicsWorld->removeRigidBody(b);
			delete b;
			b = null;
		}
	}

	//////////////////////////////////////////////////////////////////////

	btVector3 inertia(float mass, btCollisionShape *shape)
	{
		btVector3 i(0,0,0);
		shape->calculateLocalInertia(mass, i);
		return i;
	}

	//////////////////////////////////////////////////////////////////////

	btCompoundShape *InitCompoundShape(btCompoundShape *shape, btScalar *masses, btTransform &shift)
	{
		btCompoundShape* newShape(null);
		btTransform principal;
		btVector3 principalInertia;
		shape->calculatePrincipalAxisTransform(masses, principal, principalInertia);
		newShape = new btCompoundShape();
		for(int i=0; i<shape->getNumChildShapes(); i++)
		{
			btTransform newChildTransform = principal.inverse() * shape->getChildTransform(i);
			newShape->addChildShape(newChildTransform, shape->getChildShape(i));
		}
		shift = principal;
		return newShape;
	}

	//////////////////////////////////////////////////////////////////////

} // ::Physics