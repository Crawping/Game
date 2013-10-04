//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Bullet\src\BulletCollision\CollisionShapes\btShapeHull.h"

//////////////////////////////////////////////////////////////////////

static void AddQuad(btTriangleMesh *m, Vector const &a, Vector const &b, Vector const &c, Vector const &d)
{
	m->addTriangle(c, b, a, true);
	m->addTriangle(a, d, c, true);
}

//////////////////////////////////////////////////////////////////////

Track::Track(uint numControlPoints, uint step)
{
	aligned<Vector>::vector controlPoints;
	aligned<Vector>::vector track;

	controlPoints.resize(numControlPoints + 1);

	uint numTrackPoints = (numControlPoints / 3) * step + 1;

	track.resize(numTrackPoints);

	for(uint i=0; i<=numControlPoints; ++i)
	{
		float t = (float)i / (numControlPoints - 2) * PI * 2;
		float x = sinf(t) * 290;
		float y = cosf(t) * 290;
		float z = 0;

		switch(i % 3)
		{
		case 0:
			break;
		case 1:
			z += rand() / (float)RAND_MAX * 50;
			break;
		case 2:
			z -= rand() / (float)RAND_MAX * 15;
			break;
		}
		controlPoints[i] = Vec(x, y, z);
	}

	CalculateBezier(controlPoints.data(), numControlPoints, track.data(), step);

	Vector in = Vec(0.9f, 0.9f, 1);
	Vector out = Vec(1.1f, 1.1f, 1);
	Vector flr = Vec(0,0,-2);

	mTrackMesh = new btTriangleMesh();

	for(uint i=0; i<numTrackPoints; ++i )
	{
		int j = (i + 1) % numTrackPoints;	// loop

		Vector a = track[i] * in;
		Vector b = track[i] * out;
		Vector c = track[j] * out;
		Vector d = track[j] * in;

		Vector e = a + flr;
		Vector f = b + flr;
		Vector g = c + flr;
		Vector h = d + flr;

		AddQuad(mTrackMesh, a, b, c, d);
		AddQuad(mTrackMesh, a, d, h, e);
		AddQuad(mTrackMesh, b, a, e, f);

		AddQuad(mTrackMesh, c, b, f, g);
		AddQuad(mTrackMesh, h, g, f, e);
		AddQuad(mTrackMesh, d, c, g, h);
	}

	mTrackShape = new btBvhTriangleMeshShape(mTrackMesh, false);
	btDefaultMotionState *ms = new btDefaultMotionState(btTransform::getIdentity());
	mTrackBody = new btRigidBody(0, ms, mTrackShape);
	Physics::DynamicsWorld->addRigidBody(mTrackBody, 1, 2);
	mTrackBody->setActivationState(DISABLE_DEACTIVATION);
	mTrackBody->setFriction(1.0f);
	mTrackBody->setRestitution(0.0f);
}

//////////////////////////////////////////////////////////////////////

Track::~Track()
{
	Physics::DeleteRigidBody(mTrackBody);
	Delete(mTrackShape);
	Delete(mTrackMesh);
}
