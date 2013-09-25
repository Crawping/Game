//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Car;
class WheelAssembly;

//////////////////////////////////////////////////////////////////////

BEGIN_PARAMSET(WheelPairParams, "BOGUS")

	PARAM(WheelMass,				 1.00f,  0.10f, 50.00f);
	PARAM(WheelFriction,			 1.20f,  0.50f, 16.00f);
	PARAM(WheelLinearDamping,		 0.60f,  0.00f,  5.00f);
	PARAM(WheelAngularDamping,		 0.50f,  0.00f,  1.00f);
	PARAM(WheelRestitution,			 0.00f,  0.00f,  1.00f);
	PARAM(WheelOffsetX,				 2.00f, -4.00f,  4.00f);
	PARAM(WheelOffsetY,				 5.50f,  2.50f, 10.00f);
	PARAM(WheelRadius,				 2.50f,  2.00f,  5.00f);
	PARAM(WheelWidth,				 1.00f,  0.15f,  4.00f);

	PARAM(AxleMass,					 4.00f,  1.00f, 50.00f);
	PARAM(AxleHeight,				 0.25f,  1.00f,  2.00f);
	PARAM(AxleWidth,				 0.25f,  1.00f,  2.00f);
	PARAM(AxleLinearDamping,		 0.10f,  0.00f,  1.00f);
	PARAM(AxleAngularDamping,		 1.00f,  0.00f,  2.00f);

	PARAM(HubMass,					 8.00f,  1.00f, 16.00f);
	PARAM(HubWidth,					 1.00f,  0.10f,  2.00f);
	PARAM(HubHeight,				 1.00f,  0.10f,  2.00f);
	PARAM(HubLength,				 1.00f,  0.10f,  2.00f);

	PARAM(SpringLength,				 5.00f,  2.00f, 10.00f);
	PARAM(SpringStiffness,			  1200,     10,   2000);
	PARAM(SpringDamping,			0.005f, 0.0001f, 0.10f);
	PARAM(SpringLoad,				 0.75f,  0.15f,  4.50f);
	PARAM(SpringAngle,				 DEG(25), DEG(-25), DEG(60));

	PARAM(WishBoneMass,				 4.00f,  1.00f, 50.00f);
	PARAM(WishBoneWidth,			 0.60f,  0.10f,  1.50f);
	PARAM(WishBoneHeight,			 0.02f,  0.01f,  0.20f);
	PARAM(WishBoneLinearDamping,	 0.25f,  0.00f,  1.00f);
	PARAM(WishBoneAngularDamping,	 2.95f,  0.00f,  5.00f);
	PARAM(WishBoneLowerOffset,		 0.25f, -1.00f,  2.00f);
	PARAM(WishBoneUpperOffset,		 1.25f, -0.25f,  2.25f);

END_PARAMSET()

//////////////////////////////////////////////////////////////////////

BEGIN_PARAMSET(EngineParams, "Engine")

	PARAM(Power, 1, 0.5, 20);
	PARAM(Mass, 70, 1, 500);
	PARAM(VerticalOffset, 0, -5, 5);
	PARAM(HorizontalOffset, 4, -10, 10);
	PARAM(Width, 2, 0.5, 5);
	PARAM(Height, 2, 0.5, 5);
	PARAM(Length, 2, 0.5, 5);

END_PARAMSET()

//////////////////////////////////////////////////////////////////////

BEGIN_PARAMSET(CarParams, "Chassis")

	PARAM(bodyMass, 70, 1, 500);
	PARAM(bodyLength, 20, 10, 30);
	PARAM(bodyWidth, 4, 0.5f, 10);
	PARAM(bodyHeight, 3, 0.5f, 5.0f);

END_PARAMSET()

//////////////////////////////////////////////////////////////////////

class WheelAssembly
{
public:

	btCollisionShape *				mWheelShape;
	btCollisionShape *				mWheelAxleShape;
	btCollisionShape *				mHubShape;
	btCollisionShape *				mLowerWishBoneShape;
	btCollisionShape *				mUpperWishBoneShape;
	btCollisionShape *				mSpringMountShape;

	btRigidBody *					mWheel;
	btRigidBody *					mWheelAxle;
	btRigidBody *					mHub;
	btRigidBody *					mLowerWishBone;
	btRigidBody *					mUpperWishBone;
	btRigidBody *					mShockAbsorberBodyMount;
	btRigidBody *					mShockAbsorberWheelMount;

	btHingeConstraint *				mAxleHingeInner;
	btHingeConstraint *				mAxleHingeOuter;

	btHingeConstraint *				mSteeringHingeUpper;
	btHingeConstraint *				mSteeringHingeLower;

	btHingeConstraint *				mLowerWishBoneWheelFrontHinge;
	btHingeConstraint *				mUpperWishBoneWheelFrontHinge;
	btHingeConstraint *				mLowerWishBoneWheelRearHinge;
	btHingeConstraint *				mUpperWishBoneWheelRearHinge;

	btHingeConstraint *				mLowerWishBoneBodyFrontHinge;
	btHingeConstraint *				mUpperWishBoneBodyFrontHinge;
	btHingeConstraint *				mLowerWishBoneBodyRearHinge;
	btHingeConstraint *				mUpperWishBoneBodyRearHinge;

	btPoint2PointConstraint *		mShockAbsorberBodyMountHinge;
	btPoint2PointConstraint *		mShockAbsorberWheelMountHinge;
	btGeneric6DofSpringConstraint *	mShockAbsorber;

	Car *							mCar;
	bool							mIsRightWheel;
	bool							mIsRearWheel;

	WheelAssembly(bool isRightWheel, bool isRearWheel, Car &car);
	~WheelAssembly();

	void Create(WheelPairParams *p, btVector3 const &carWorldPos, btVector3 const &carSize);
	void Destroy();
	float LateralSlip();	// how much is this wheel sliding sideways
	float Skid();			// -ive = skidding, +ive = wheels spinning
	void ApplyParameters(WheelPairParams *params);

};

//////////////////////////////////////////////////////////////////////

class Car
{
public:

	enum WheelID
	{
		BackLeft,
		BackRight,
		FrontLeft,
		FrontRight,
		NumWheels
	};

	EngineParams				mEngineParams;
	CarParams					mCarParams;
	WheelPairParams				mRearWheelParams;
	WheelPairParams				mFrontWheelParams;
	ParameterSetCollection		mParameterSets;

	btCollisionShape *			mEngineShape;
	btCollisionShape *			mBodyShape;
	btCompoundShape *			mBodyAndEngineShape;
	btRigidBody *				mBody;
	WheelAssembly *				mWheelAssembly[NumWheels];

	Car();
	~Car();

	void Create();
	void Destroy();
	void ApplyPower(float speed, float force);
	void AntiRoll();
	void ApplyAntiRoll(WheelAssembly *left, WheelAssembly *right);

	void ApplyParameters();

	bool IsValid() const
	{
		return mBody != null;
	}
};

