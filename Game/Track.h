//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

ALIGN(16, class) Track
{

public:

	Track(uint numControlPoints, uint step);
	~Track();

private:

	aligned<Vector>::vector			mControlPoints;
	aligned<Vector>::vector			mTrack;

	vector<btConvexHullShape *>		mTrackSectionShapes;
	vector<btRigidBody *>			mTrackSections;

};