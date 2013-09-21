//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Car;
class WheelAssembly;

//////////////////////////////////////////////////////////////////////

class PhysicalObject
{
public:

	btDynamicsWorld *				mDW;

	PhysicalObject(btDynamicsWorld *world)
	{
		mDW = world;
	}

	~PhysicalObject()
	{
	}

	template <typename T> void DeleteConstraint(T * &c)
	{
		if(c != null)
		{
			mDW->removeConstraint(c);
			delete c;
			c = null;
		}
	}

	void DeleteRigidBody(btRigidBody * &b)
	{
		if(b != null)
		{
			mDW->removeRigidBody(b);
			delete b;
			b = null;
		}
	}
};

//////////////////////////////////////////////////////////////////////

struct Parameter
{
	static int numParameters;

	string strName;
	float mValue;
	float minValue;
	float maxValue;
	float defaultValue;

	uint32 index;
	uint32 mask;		// what mode can this parameter be edited in?

	Parameter()
		: index(++numParameters)
	{
	}

	Parameter(char const *name, float val, float minVal, float maxVal, float defaultVal)
		: strName(name)
		, mValue(val)
		, minValue(minVal)
		, maxValue(maxVal)
		, defaultValue(defaultVal)
		, index(++numParameters)
	{
	}

	string ToXMLString() const
	{
		return Format("\t\t<Parameter Name=\"%s\" Value=\"%f\" MinValue=\"%f\" MaxValue=\"%f\" DefaultValue=\"%f\" />\n", strName.c_str(), mValue, minValue, maxValue, defaultValue);
	}

	operator float()
	{
		return mValue;
	}
	void operator *= (float f)
	{
		mValue *= f;
	}

	void operator = (float const &f)
	{
	mValue = f;
	}
};

//////////////////////////////////////////////////////////////////////

struct ParameterSet
{
	static vector<ParameterSet *> mSets;
	static vector<Parameter *> mParameterList;

	vector<Parameter *> mParameters;
	string mName;

	void CopyParameterSet(char const *name)
	{
		mName = name;
		mSets.push_back(this);
		mParameters = mParameterList;	// Yoink!
		mParameterList.clear();
	}

	string ToXMLString() const
	{
		string s = Format("\t<ParameterSet Name=\"%s\" ParameterCount=\"%d\">\n", mName.c_str(), mParameters.size());
		for(auto p:mParameters)
		{
			s += p->ToXMLString();
		}
		s += "\t</ParameterSet>\n";
		return s;
	}

	void SetFromXML(XmlDocument *xmlDocument, char const *name);
};

//////////////////////////////////////////////////////////////////////

#define PNAME(a) s_ ## a

#define PARAM(name, _mask, _def, _min, _max)	\
										\
struct PNAME(name) : Parameter			\
{										\
	PNAME(name)()						\
		: Parameter()					\
	{									\
		mValue = _def;					\
		mask = _mask;					\
		defaultValue = _def;			\
		minValue = _min;				\
		maxValue = _max;				\
		strName = string(#name);		\
		mParameterList.push_back(this);	\
	}									\
};										\
PNAME(name) name

//////////////////////////////////////////////////////////////////////

#define CAR_EDITMASK	1
#define	CAR_DRIVEMASK	2
#define	CAR_ALLMASK		(CAR_EDITMASK | CAR_DRIVEMASK)

struct WheelPairParams : ParameterSet
{
	PARAM(WheelMass,				CAR_ALLMASK,  1.00f,  0.10f, 50.00f);
	PARAM(WheelFriction,			CAR_ALLMASK,  1.20f,  0.50f, 16.00f);
	PARAM(WheelLinearDamping,		CAR_ALLMASK,  0.60f,  0.00f,  5.00f);
	PARAM(WheelAngularDamping,		CAR_ALLMASK,  0.50f,  0.00f,  1.00f);
	PARAM(WheelRestitution,			CAR_ALLMASK,  0.00f,  0.00f,  1.00f);
	PARAM(AxleMass,					CAR_ALLMASK,  4.00f,  1.00f, 50.00f);
	PARAM(AxleLinearDamping,		CAR_ALLMASK,  0.10f,  0.00f,  1.00f);
	PARAM(AxleAngularDamping,		CAR_ALLMASK,  1.00f,  0.00f,  2.00f);
	PARAM(WishBoneMass,				CAR_ALLMASK,  4.00f,  1.00f, 50.00f);
	PARAM(SteeringArmMass,			CAR_ALLMASK,  8.00f,  1.00f, 16.00f);
	PARAM(WishBoneLinearDamping,	CAR_ALLMASK,  0.25f,  0.00f,  1.00f);
	PARAM(WishBoneAngularDamping,	CAR_ALLMASK,  2.95f,  0.00f,  5.00f);
	PARAM(SpringStiffness,			CAR_ALLMASK,   1200,     10,   2000);
	PARAM(SpringDamping,			CAR_ALLMASK, 0.005f, 0.0001f, 0.10f);
	PARAM(SpringLoad,				CAR_ALLMASK,  0.75f,  0.15f,  4.50f);

	PARAM(WheelOffsetX,				CAR_EDITMASK, 2.00f, -4.00f,  4.00f);
	PARAM(WheelOffsetY,				CAR_EDITMASK, 5.50f,  2.50f, 10.00f);
	PARAM(WheelRadius,				CAR_EDITMASK, 2.50f,  2.00f,  5.00f);
	PARAM(WheelWidth,				CAR_EDITMASK, 1.00f,  0.15f,  4.00f);
	PARAM(AxleHeight,				CAR_EDITMASK, 0.25f,  1.00f,  2.00f);
	PARAM(AxleWidth,				CAR_EDITMASK, 0.25f,  1.00f,  2.00f);
	PARAM(WishBoneWidth,			CAR_EDITMASK, 0.60f,  0.10f,  1.50f);

	PARAM(WishBoneHeight,			CAR_EDITMASK, 0.02f,  0.01f,  0.20f);
	PARAM(SteeringArmWidth,			CAR_EDITMASK, 1.00f,  0.10f,  2.00f);
	PARAM(SteeringArmHeight,		CAR_EDITMASK, 1.00f,  0.10f,  2.00f);
	PARAM(SteeringArmLength,		CAR_EDITMASK, 1.00f,  0.10f,  2.00f);
	PARAM(WishBoneLowerOffset,		CAR_EDITMASK, 0.25f, -1.00f,  2.00f);
	PARAM(WishBoneUpperOffset,		CAR_EDITMASK, 1.25f, -0.25f,  2.25f);
	PARAM(SpringLength,				CAR_EDITMASK, 5.00f,  2.00f, 10.00f);

	PARAM(SpringAngle,				CAR_EDITMASK, (SIMD_PI / 180) * 25, (SIMD_PI / 180) * -25, (SIMD_PI / 180) * 60);

	WheelPairParams(char const *name)
	{
		CopyParameterSet(name);
	}
};

//////////////////////////////////////////////////////////////////////

struct CarParams : ParameterSet
{
	PARAM(bodyMass, CAR_ALLMASK, 70, 1, 500);
	PARAM(bodyLength, CAR_EDITMASK, 20, 10, 30);
	PARAM(bodyWidth, CAR_EDITMASK, 4, 0.5f, 10);
	PARAM(bodyHeight, CAR_EDITMASK, 3, 0.5f, 5.0f);

	CarParams(char const *name)
	{
		CopyParameterSet(name);
	}
};

//////////////////////////////////////////////////////////////////////

class WheelAssembly : PhysicalObject
{
public:

	btCollisionShape *				mWheelShape;
	btCollisionShape *				mWheelAxleShape;
	btCollisionShape *				mSteeringArmShape;
	btCollisionShape *				mLowerWishBoneShape;
	btCollisionShape *				mUpperWishBoneShape;
	btCollisionShape *				mSpringMountShape;

	btRigidBody *					mWheel;
	btRigidBody *					mWheelAxle;
	btRigidBody *					mSteeringArm;
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

	WheelAssembly(btDynamicsWorld *dynamicsWorld, bool isRightWheel, bool isRearWheel, Car &car);
	~WheelAssembly();

	void Create(WheelPairParams *p);
	void Destroy();
	float LateralSlip();	// how much is this wheel sliding sideways
	float Skid();			// -ive = skidding, +ive = wheels spinning
	void ApplyParameters(WheelPairParams *params);

};

//////////////////////////////////////////////////////////////////////

class Car : PhysicalObject
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

	CarParams					mCarParams;
	WheelPairParams				mFrontWheelParams;
	WheelPairParams				mRearWheelParams;
	btCollisionShape *			mBodyShape;
	btRigidBody *				mBody;
	WheelAssembly *				mWheelAssembly[NumWheels];

	Car(btDynamicsWorld *dynamicsWorld);
	~Car();

	void Create();
	void Destroy();
	void ApplyPower(float speed, float force);
	void AntiRoll();
	void ApplyAntiRoll(WheelAssembly *left, WheelAssembly *right);

	void ApplyParameters();
	string ToXMLString() const;
	void SetParametersFromXML(char *fileContents);

	bool IsValid() const
	{
		return mBody != null;
	}
};

