//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Vehicle
{
public:

	//////////////////////////////////////////////////////////////////////

	class Wheel
	{
	public:

		Wheel(float radius, float width, Vec4f offset)
			: mShape(null)
			, mRadius(radius)
			, mWidth(width)
			, mOffset(offset)
		{
			mShape = new btCylinderShapeX(Vec4(width, radius, radius));
		}

		~Wheel()
		{
		}

		Vec4f				mOffset;	// current position relative to car origin, in car space
		float				mRadius;
		float				mWidth;
		btCylinderShapeX *	mShape;

	};

	//////////////////////////////////////////////////////////////////////

	Vehicle(Vec4f position, float width, float length, float height)
		: mShape(null)
		, mBody(null)
		, mWidth(width)
		, mLength(length)
		, mHeight(height)
	{
		mShape = new btBoxShape(Vec4(width, length, height));
		btTransform t(btQuaternion::getIdentity(), position);
		btDefaultMotionState *ms = new btDefaultMotionState(t);
		mBody = new btRigidBody(1.0f, ms, mShape, Physics::inertia(1.0f, mShape));
		Physics::DynamicsWorld->addRigidBody(mBody, -1, -1);	// collide with everything
	}

	~Vehicle()
	{
		Physics::DeleteRigidBody(mBody);
		Delete(mShape);
	}

	void Draw()
	{
//		Matrix transform = Physics::ConvertTransformToMatrix(mBody->getWorldTransform());
	}

	float					mWidth;		// X
	float					mLength;	// Y
	float					mHeight;	// Z
	btBoxShape *			mShape;
	btRigidBody *			mBody;		// use the transform and the wheel offsets to do raycasting
};
