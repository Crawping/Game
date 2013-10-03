//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

Track::Track(uint numControlPoints, uint step)
{
	mControlPoints.resize(numControlPoints);

	uint numTrackPoints = (numControlPoints / 3) * step + 1;

	mTrack.resize(numTrackPoints);

	for(uint i=0; i<=numControlPoints; ++i)
	{
		float t = (float)i / (numControlPoints - 1) * PI * 2;
		float x = sinf(t) * 290;
		float y = cosf(t) * 290;
		float z = 50;

		switch(i % 3)
		{
		case 0:
			break;
		case 1:
			z += rand() / (float)RAND_MAX * 35;
			break;
		case 2:
			z -= rand() / (float)RAND_MAX * 35;
			break;
		}
		mControlPoints[i] = Vec(x, y, z);
	}

	CalculateBezier(mControlPoints.data(), numControlPoints, mTrack.data(), step);

	Vector in = Vec(0.9f, 0.9f, 1);
	Vector out = Vec(1.1f, 1.1f, 1);

	for(uint i=0; i<numTrackPoints; ++i)
	{
		int j = (i + 1) % numTrackPoints;	// loop

		Vector a = mTrack[i] * in;
		Vector b = mTrack[i] * in;
		Vector c = mTrack[j] * out;
		Vector d = mTrack[j] * out;

		btConvexHullShape *hull = new btConvexHullShape();
		hull->addPoint(a);
		hull->addPoint(b);
		hull->addPoint(c);
		hull->addPoint(d);
		mTrackSectionShapes.push_back(hull);

		btDefaultMotionState *ms = new btDefaultMotionState(btTransform::getIdentity());
		btRigidBody *body = new btRigidBody(0, ms, hull);
		Physics::DynamicsWorld->addRigidBody(body);
		body->setActivationState(DISABLE_DEACTIVATION);
		body->setFriction(1.0f);
		body->setRestitution(0.0f);
		mTrackSections.push_back(body);
	}
}

//////////////////////////////////////////////////////////////////////

Track::~Track()
{
	for(auto b: mTrackSections)
	{
		Physics::DeleteRigidBody(b);
	}
	for(auto s: mTrackSectionShapes)
	{
		Delete(s);
	}
}
