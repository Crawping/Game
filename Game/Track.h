//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

ALIGN(16, class) Track
{

public:

	Track(uint numControlPoints, uint step);
	~Track();

private:

	btTriangleMesh *				mTrackMesh;
	btBvhTriangleMeshShape *		mTrackShape;
	btRigidBody *					mTrackBody;

};