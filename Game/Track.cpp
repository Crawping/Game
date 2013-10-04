//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

static void AddQuad(btTriangleMesh *m, Vec4f const &a, Vec4f const &b, Vec4f const &c, Vec4f const &d)
{
	m->addTriangle(c, b, a, true);
	m->addTriangle(a, d, c, true);
}

//////////////////////////////////////////////////////////////////////

Track::Track(uint numControlPoints, uint step)
{
	// setup arrays of control points and track points
	aligned<Vec4f>::vector controlPoints;
	aligned<Vec4f>::vector trackPoints;
	uint numTrackPoints = (numControlPoints / 3) * step + 1;
	controlPoints.resize(numControlPoints);
	trackPoints.resize(numTrackPoints);

	// build circular undulating track
	for(uint i=0; i<numControlPoints; ++i)
	{
		float t = i * PI * 2 / (numControlPoints - 2);
		float x = sinf(t) * 290;
		float y = cosf(t) * 290;
		float z;
		switch(i % 3)
		{
			case 1:	z = rand() / (float)RAND_MAX * 50;	break;
			case 2:	z = rand() / (float)RAND_MAX * -15;	break;
			default: z = 0;
		}
		controlPoints[i] = Vec4(x, y, z);
	}
	CalculateBezier(controlPoints.data(), numControlPoints, trackPoints.data(), step);

	// make the mesh shape
	Vec4f in = Vec4(0.9f, 0.9f, 1);
	Vec4f out = Vec4(1.1f, 1.1f, 1);
	Vec4f flr = Vec4(0,0,-2);
	mTrackMesh = new btTriangleMesh();
	for(uint i=0; i<numTrackPoints; ++i )
	{
		int j = (i + 1) % numTrackPoints;
		Vec4f a = trackPoints[i] * in;
		Vec4f b = trackPoints[i] * out;
		Vec4f c = trackPoints[j] * out;
		Vec4f d = trackPoints[j] * in;
		AddQuad(mTrackMesh, a, b, c, d);
	}
	mTrackShape = new btBvhTriangleMeshShape(mTrackMesh, true, true);

	// make the rigidbody
	mTrackBody = new btRigidBody(0, new btDefaultMotionState(btTransform::getIdentity()), mTrackShape);
	mTrackBody->setActivationState(DISABLE_DEACTIVATION);
	mTrackBody->setFriction(1.0f);
	mTrackBody->setRestitution(0.0f);
	Physics::DynamicsWorld->addRigidBody(mTrackBody, 1, 2);
}

//////////////////////////////////////////////////////////////////////

Track::~Track()
{
	Physics::DeleteRigidBody(mTrackBody);
	Delete(mTrackShape);
	Delete(mTrackMesh);
}
