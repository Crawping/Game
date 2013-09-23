//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Car.h"

//////////////////////////////////////////////////////////////////////

Car::Car(btDynamicsWorld *dynamicsWorld)
	: PhysicalObject(dynamicsWorld)
	, mBody(null)
	, mBodyShape(null)
	, mCarParams("Chassis")
	, mEngineParams("Engine")
	, mFrontWheelParams("Front Wheels")
	, mRearWheelParams("Rear Wheels")
{
	mWheelAssembly[BackRight] = new WheelAssembly(mDW, true, true, *this);
	mWheelAssembly[BackLeft] = new WheelAssembly(mDW, false, true, *this);
	mWheelAssembly[FrontRight] = new WheelAssembly(mDW, true, false, *this);
	mWheelAssembly[FrontLeft] = new WheelAssembly(mDW, false, false, *this);
}

//////////////////////////////////////////////////////////////////////

string Car::ToXMLString() const
{
	string s("<xml>\n");
	s += mEngineParams.ToXMLString();
	s += mCarParams.ToXMLString();
	s += mRearWheelParams.ToXMLString();
	s += mFrontWheelParams.ToXMLString();
	s += "</xml>\n";
	return s;
}

//////////////////////////////////////////////////////////////////////

void Car::SetParametersFromXML(char *fileContents)
{
	XmlDocument xml;
	try
	{
		xml.parse<rapidxml::parse_default>(fileContents);
		mEngineParams.SetFromXML(&xml, "Engine");
		mCarParams.SetFromXML(&xml, "Chassis");
		mRearWheelParams.SetFromXML(&xml, "Rear Wheels");
		mFrontWheelParams.SetFromXML(&xml, "Front Wheels");
	}
	catch(rapidxml::parse_error e)
	{
		TRACE("Error parsing XML: %s\n", e.what());
	}
}

//////////////////////////////////////////////////////////////////////

WheelAssembly::WheelAssembly(btDynamicsWorld *dynamicsWorld, bool isRightWheel, bool isRearWheel, Car &car)
	: PhysicalObject(dynamicsWorld)
	, mAxleHingeInner(null)
	, mAxleHingeOuter(null)
	, mHub(null)
	, mLowerWishBoneWheelFrontHinge(null)
	, mUpperWishBoneWheelFrontHinge(null)
	, mLowerWishBoneBodyFrontHinge(null)
	, mUpperWishBoneBodyFrontHinge(null)
	, mLowerWishBoneWheelRearHinge(null)
	, mUpperWishBoneWheelRearHinge(null)
	, mLowerWishBoneBodyRearHinge(null)
	, mUpperWishBoneBodyRearHinge(null)
	, mShockAbsorberBodyMountHinge(null)
	, mShockAbsorberWheelMountHinge(null)
	, mSteeringHingeUpper(null)
	, mSteeringHingeLower(null)
	, mShockAbsorber(null)
	, mWheel(null)
	, mWheelAxle(null)
	, mLowerWishBone(null)
	, mUpperWishBone(null)
	, mShockAbsorberBodyMount(null)
	, mShockAbsorberWheelMount(null)
	, mWheelShape(null)
	, mWheelAxleShape(null)
	, mHubShape(null)
	, mLowerWishBoneShape(null)
	, mUpperWishBoneShape(null)
	, mSpringMountShape(null)
	, mCar(&car)
	, mIsRightWheel(isRightWheel)
	, mIsRearWheel(isRearWheel)
{
}

//////////////////////////////////////////////////////////////////////

btVector3 inertia(float mass, btCollisionShape *shape)
{
	btVector3 i(0,0,0);
	shape->calculateLocalInertia(mass, i);
	return i;
}

//////////////////////////////////////////////////////////////////////

void Car::Create()
{
	Destroy();

	Vec3 carPos(0, 0, 5);
	Vec3 carSize(mCarParams.bodyLength, mCarParams.bodyWidth, mCarParams.bodyHeight);
	float bodyMass = mCarParams.bodyMass;

	mBodyShape = new btBoxShape(btVector3(carSize.x / 2, carSize.y / 2, carSize.z / 2));
	btTransform bodyTransform(btQuaternion::getIdentity(), btVector3(carPos.x, carPos.y, carPos.z));
	btDefaultMotionState *bodyMotionState = new btDefaultMotionState(bodyTransform);
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(bodyMass, bodyMotionState, mBodyShape, inertia(bodyMass, mBodyShape));
	mBody = new btRigidBody(bodyInfo);
	mDW->addRigidBody(mBody, 4, 1);

	mWheelAssembly[FrontLeft]->Create(&mFrontWheelParams);
	mWheelAssembly[FrontRight]->Create(&mFrontWheelParams);
	mWheelAssembly[BackLeft]->Create(&mRearWheelParams);
	mWheelAssembly[BackRight]->Create(&mRearWheelParams);
	ApplyParameters();
}

//////////////////////////////////////////////////////////////////////

void Car::ApplyParameters()
{
	mBody->setMassProps(mCarParams.bodyMass, inertia(mCarParams.bodyMass, mBodyShape));

	mWheelAssembly[FrontLeft]->ApplyParameters(&mFrontWheelParams);
	mWheelAssembly[FrontRight]->ApplyParameters(&mFrontWheelParams);
	mWheelAssembly[BackLeft]->ApplyParameters(&mRearWheelParams);
	mWheelAssembly[BackRight]->ApplyParameters(&mRearWheelParams);
}

//////////////////////////////////////////////////////////////////////

void WheelAssembly::Create(WheelPairParams *p)
{
	float xReflect = mIsRearWheel ? 1.0f : -1.0f;
	float yReflect = mIsRightWheel ? 1.0f : -1.0f;
	btVector3 const &carWorldPos = mCar->mBody->getWorldTransform().getOrigin();
	btVector3 const &carSize = ((btBoxShape *)mCar->mBodyShape)->getHalfExtentsWithMargin();
	float carLength = carSize.x();
	float carWidth = carSize.y();
	float carHeight = carSize.z();
	Vec3 wheelOffset((carLength - p->WheelOffsetX) * xReflect, (carWidth + p->WheelOffsetY) * -yReflect, 0);
	btVector3 wheelPos = btVector3(wheelOffset.x, wheelOffset.y, wheelOffset.z) + carWorldPos;
	float wheelRadius = p->WheelRadius;
	float wheelWidth = p->WheelWidth;
	float wheelMass = p->WheelMass;
	float wheelFriction = p->WheelFriction;
	float wheelLinearDamping = p->WheelLinearDamping;
	float wheelAngularDamping = p->WheelAngularDamping;
	float wheelRestitution = p->WheelRestitution;
	float axleHeight = p->AxleHeight;
	float axleWidth = p->AxleWidth;
	float axleMass = p->AxleMass;
	float axleLinearDamping = p->AxleLinearDamping;
	float axleAngularDamping = p->AxleAngularDamping;
	float wishBoneMass = p->WishBoneMass;
	float wishBoneWidth = p->WishBoneWidth;
	float wishBoneHeight = p->WishBoneHeight;
	float steeringArmMass = p->HubMass;
	float steeringArmHeight = p->HubHeight;
	float wishBoneLinearDamping = p->WishBoneLinearDamping;
	float wishBoneAngularDamping = p->WishBoneAngularDamping;
	float lowerBodyWishBoneZOffset = p->WishBoneLowerOffset;
	float upperBodyWishBoneZOffset = p->WishBoneUpperOffset;
	float springLength = p->SpringLength;
	float springStiffness = p->SpringStiffness;
	float springDamping = p->SpringDamping;
	float springLoad = p->SpringLoad;
	float angle = p->SpringAngle;	
	float axleLength = wheelWidth;
	float steeringArmWidth = p->HubWidth;
	float steeringArmLength = p->HubLength;
	float upperWishBoneZWheelOffset = steeringArmHeight;
	float lowerWishBoneZWheelOffset = -steeringArmHeight;
	float upperWishBoneZBodyOffset = upperWishBoneZWheelOffset + upperBodyWishBoneZOffset;
	float lowerWishBoneZBodyOffset = lowerWishBoneZWheelOffset + lowerBodyWishBoneZOffset;

	// Create the wheel
	mWheelShape = new btCylinderShape(btVector3(wheelRadius, wheelWidth, 0.0f));
	btTransform wheelTransform(btQuaternion::getIdentity(), wheelPos);
	btDefaultMotionState * wheelMotionState = new btDefaultMotionState(wheelTransform);
	mWheel = new btRigidBody(wheelMass, wheelMotionState, mWheelShape, inertia(wheelMass, mWheelShape));
	mDW->addRigidBody(mWheel, 2, 1);
	mWheel->setDamping(wheelLinearDamping, wheelAngularDamping);
	mWheel->setFriction(wheelFriction);
	mWheel->setRestitution(wheelRestitution);
	mWheel->setActivationState(DISABLE_DEACTIVATION);

	// Create the axle
	mWheelAxleShape = new btBoxShape(btVector3(axleWidth, axleLength, axleHeight));
	btTransform hubTransform(btQuaternion::getIdentity(), wheelPos);
	btDefaultMotionState *hubMotionState = new btDefaultMotionState(hubTransform);
	mWheelAxle = new btRigidBody(axleMass, hubMotionState, mWheelAxleShape, inertia(axleMass, mWheelAxleShape));
	mDW->addRigidBody(mWheelAxle, 0, 0);
	mWheelAxle->setDamping(axleLinearDamping, axleAngularDamping);

	// Create hinges joining wheel to hub
	mAxleHingeInner = new btHingeConstraint(*mWheelAxle, *mWheel, btVector3(0, wheelWidth, 0), btVector3(0, wheelWidth, 0), btVector3(0,-1,0), btVector3(0,-1,0), true);
	mAxleHingeOuter = new btHingeConstraint(*mWheelAxle, *mWheel, btVector3(0, -wheelWidth, 0), btVector3(0, -wheelWidth, 0), btVector3(0,-1,0), btVector3(0,-1,0), true);

	mDW->addConstraint(mAxleHingeInner, true);
	mDW->addConstraint(mAxleHingeOuter, true);

	// create the steering arm
	mHubShape = new btBoxShape(btVector3(steeringArmLength, steeringArmWidth, steeringArmHeight));
	btTransform steeringArmTransform(btQuaternion::getIdentity(), wheelPos);
	btDefaultMotionState *steeringArmMotionState = new btDefaultMotionState(steeringArmTransform);
	mHub = new btRigidBody(steeringArmMass, steeringArmMotionState, mHubShape, inertia(steeringArmMass, mHubShape));
	mDW->addRigidBody(mHub, 0, 0);

	// join the steering arm to the axle
	mSteeringHingeUpper = new btHingeConstraint(*mWheelAxle, *mHub, btVector3(0,0,steeringArmHeight), btVector3(0, 0, steeringArmHeight), btVector3(0,0,1), btVector3(0,0,1), true);
	mSteeringHingeLower = new btHingeConstraint(*mWheelAxle, *mHub, btVector3(0,0,-steeringArmHeight), btVector3(0, 0, -steeringArmHeight), btVector3(0,0,1), btVector3(0,0,1), true);

	mDW->addConstraint(mSteeringHingeUpper, true);
	mDW->addConstraint(mSteeringHingeLower, true);

	mSteeringHingeUpper->setLimit(0,0);
	mSteeringHingeLower->setLimit(0,0);

	btVector3 const &hubPosition = wheelPos;
	btVector3 armJoinPos(hubPosition);

	// FFS: Make this a function!
	{

		btVector3 lowerBodyJoinPosition = btVector3(hubPosition.x(), carWidth * -yReflect, hubPosition.z() + lowerBodyWishBoneZOffset);
		btVector3 upperBodyJoinPosition = btVector3(hubPosition.x(), carWidth * -yReflect, hubPosition.z() + upperBodyWishBoneZOffset);

		btVector3 lowerArmDelta = armJoinPos - lowerBodyJoinPosition;
		btVector3 upperArmDelta = armJoinPos - upperBodyJoinPosition;

		float lowerWishBoneLength = lowerArmDelta.length();
		float upperWishBoneLength = upperArmDelta.length();

		float lowerWishBoneAngle = asinf(lowerArmDelta.z() / lowerWishBoneLength) * yReflect;
		float upperWishBoneAngle = asinf(upperArmDelta.z() / upperWishBoneLength) * yReflect;

		btVector3 lowerArmOrigin = (armJoinPos + lowerBodyJoinPosition) / 2;
		btVector3 upperArmOrigin = (armJoinPos + upperBodyJoinPosition) / 2;

		// Create the lower wishbones
		mLowerWishBoneShape = new btBoxShape(btVector3(wishBoneWidth, lowerWishBoneLength / 2, wishBoneHeight));
		mUpperWishBoneShape = new btBoxShape(btVector3(wishBoneWidth, upperWishBoneLength / 2, wishBoneHeight));

		btTransform lowerArmTransform(btQuaternion(btVector3(1,0,0), -lowerWishBoneAngle), btVector3(lowerArmOrigin + btVector3(0,0,lowerWishBoneZWheelOffset)));
		btTransform upperArmTransform(btQuaternion(btVector3(1,0,0), -upperWishBoneAngle), btVector3(upperArmOrigin + btVector3(0,0,upperWishBoneZWheelOffset)));

		btDefaultMotionState *lowerArmMotionState = new btDefaultMotionState(lowerArmTransform);
		btDefaultMotionState *upperArmMotionState = new btDefaultMotionState(upperArmTransform);

		mLowerWishBone = new btRigidBody(wishBoneMass, lowerArmMotionState, mLowerWishBoneShape, inertia(wishBoneMass, mLowerWishBoneShape));
		mUpperWishBone = new btRigidBody(wishBoneMass, upperArmMotionState, mUpperWishBoneShape, inertia(wishBoneMass, mUpperWishBoneShape));

		mDW->addRigidBody(mLowerWishBone, 0, 0);
		mDW->addRigidBody(mUpperWishBone, 0, 0);

		mUpperWishBone->setDamping(wishBoneLinearDamping, wishBoneAngularDamping);
		mLowerWishBone->setDamping(wishBoneLinearDamping, wishBoneAngularDamping);

		float lim = SIMD_PI / 4;
		float lo = -lim;
		float hi = lim;

		// Join Position relative to the car body
		btVector3 lowerBodyFrontJointPos = lowerBodyJoinPosition + btVector3(wishBoneWidth, 0, -steeringArmHeight ) - carWorldPos;
		btVector3 lowerBodyRearJointPos = lowerBodyJoinPosition + btVector3(-wishBoneWidth, 0, -steeringArmHeight ) - carWorldPos;
		btVector3 upperBodyFrontJointPos = upperBodyJoinPosition + btVector3(wishBoneWidth, 0, +steeringArmHeight ) - carWorldPos;
		btVector3 upperBodyRearJointPos = upperBodyJoinPosition + btVector3(-wishBoneWidth, 0, +steeringArmHeight ) - carWorldPos;

		btVector3 lowerArmFrontJoinPos(wishBoneWidth, 0, lowerWishBoneZWheelOffset);
		btVector3 upperArmFrontJoinPos(wishBoneWidth, 0, upperWishBoneZWheelOffset);
		btVector3 lowerArmRearJoinPos(-wishBoneWidth, 0, lowerWishBoneZWheelOffset);
		btVector3 upperArmRearJoinPos(-wishBoneWidth, 0, upperWishBoneZWheelOffset);

		float lwbl = lowerWishBoneLength * 0.5f;
		float uwbl = upperWishBoneLength * 0.5f;

		mLowerWishBoneWheelFrontHinge = new btHingeConstraint(*mHub, *mLowerWishBone, lowerArmFrontJoinPos, btVector3(wishBoneWidth, lwbl * -yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mLowerWishBoneBodyFrontHinge = new btHingeConstraint(*mCar->mBody, *mLowerWishBone, lowerBodyFrontJointPos, btVector3(wishBoneWidth, lwbl * yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mLowerWishBoneWheelRearHinge = new btHingeConstraint(*mHub, *mLowerWishBone, lowerArmRearJoinPos, btVector3(-wishBoneWidth, lwbl * -yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mLowerWishBoneBodyRearHinge = new btHingeConstraint(*mCar->mBody, *mLowerWishBone, lowerBodyRearJointPos, btVector3(-wishBoneWidth, lwbl * yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);

		mUpperWishBoneWheelFrontHinge = new btHingeConstraint(*mHub, *mUpperWishBone, upperArmFrontJoinPos, btVector3(wishBoneWidth, uwbl * -yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mUpperWishBoneBodyFrontHinge = new btHingeConstraint(*mCar->mBody, *mUpperWishBone, upperBodyFrontJointPos, btVector3(wishBoneWidth, uwbl * yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mUpperWishBoneWheelRearHinge = new btHingeConstraint(*mHub, *mUpperWishBone, upperArmRearJoinPos, btVector3(-wishBoneWidth, uwbl * -yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mUpperWishBoneBodyRearHinge = new btHingeConstraint(*mCar->mBody, *mUpperWishBone, upperBodyRearJointPos, btVector3(-wishBoneWidth, uwbl * yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);

		mDW->addConstraint(mLowerWishBoneWheelFrontHinge, true);
		mDW->addConstraint(mUpperWishBoneWheelFrontHinge, true);
		mDW->addConstraint(mLowerWishBoneBodyFrontHinge, true);
		mDW->addConstraint(mUpperWishBoneBodyFrontHinge, true);

		mDW->addConstraint(mLowerWishBoneWheelRearHinge, true);
		mDW->addConstraint(mUpperWishBoneWheelRearHinge, true);
		mDW->addConstraint(mLowerWishBoneBodyRearHinge, true);
		mDW->addConstraint(mUpperWishBoneBodyRearHinge, true);

		mLowerWishBoneWheelFrontHinge->setLimit(lo, hi);
		mUpperWishBoneWheelFrontHinge->setLimit(lo, hi);
		mLowerWishBoneBodyFrontHinge->setLimit(lo, hi);
		mUpperWishBoneBodyFrontHinge->setLimit(lo, hi);

		mLowerWishBoneWheelRearHinge->setLimit(lo, hi);
		mUpperWishBoneWheelRearHinge->setLimit(lo, hi);
		mLowerWishBoneBodyRearHinge->setLimit(lo, hi);
		mUpperWishBoneBodyRearHinge->setLimit(lo, hi);
	}

	// FFS: Make the above a function

	// Create spring mount
	float springMountShapeMass = 0.1f;
	mSpringMountShape = new btBoxShape(btVector3(0.125f, 0.125f, 0.125f));

	btTransform spA;
	btTransform spB;
	spA.setIdentity();
	spB.setIdentity();

	// hub position relative to car body position
	btVector3 springOffset(0, 0, 0);
	btVector3 springJoinPos;
	springJoinPos = hubPosition + springOffset - carWorldPos;
	btQuaternion q(0, SIMD_PI / 2 - angle * yReflect, 0);
	btMatrix3x3 m(q);
	spA.setBasis(m);
	spB.setBasis(m);
	btTransform rot(q);
	btVector3 point = rot.getBasis().getColumn(1);
	springJoinPos += point * springLength;
	spB.setOrigin(springJoinPos);

	// Create Spring Mount on the body of the car
	btTransform springMountBodyTransform(btQuaternion::getIdentity(), springJoinPos + carWorldPos);
	btDefaultMotionState *springMountBodyMotionState = new btDefaultMotionState(springMountBodyTransform);
	mShockAbsorberBodyMount = new btRigidBody(springMountShapeMass, springMountBodyMotionState, mSpringMountShape, inertia(springMountShapeMass, mSpringMountShape));
	mDW->addRigidBody(mShockAbsorberBodyMount, 0, 0);

	// Create Spring Mount on the axle
	btTransform springMountWheelTransform(btQuaternion::getIdentity(), armJoinPos);
	btDefaultMotionState *springMountWheelMotionState = new btDefaultMotionState(springMountWheelTransform);
	mShockAbsorberWheelMount = new btRigidBody(springMountShapeMass, springMountWheelMotionState, mSpringMountShape, inertia(springMountShapeMass, mSpringMountShape));
	mDW->addRigidBody(mShockAbsorberWheelMount, 0, 0);

	// Attach Spring Mount to the body of the car with a free hinge
	mShockAbsorberBodyMountHinge = new btPoint2PointConstraint(*mCar->mBody, *mShockAbsorberBodyMount, springJoinPos, btVector3(0,0,0));
	mDW->addConstraint(mShockAbsorberBodyMountHinge, true);

	// Attach Spring Mount to the steering arm with a free hinge
	mShockAbsorberWheelMountHinge = new btPoint2PointConstraint(*mHub, *mShockAbsorberWheelMount, springOffset, btVector3(0,0,0));
	mDW->addConstraint(mShockAbsorberWheelMountHinge, true);

	// Attach the 2 Spring Mounts to each other with a spring
	btVector3 a = mShockAbsorberWheelMount->getWorldTransform().getOrigin();
	btVector3 b = mShockAbsorberBodyMount->getWorldTransform().getOrigin();
	btMatrix3x3 ma = GetOrientedBasis((a-b).normalize());
	mShockAbsorberBodyMount->getWorldTransform().setBasis(ma);
	mShockAbsorberWheelMount->getWorldTransform().setBasis(ma);

	btTransform ta(btQuaternion::getIdentity(), btVector3(0,0,springLength));
	btTransform tb(btQuaternion::getIdentity(), btVector3(0,0,0));
	mShockAbsorber = new btGeneric6DofSpringConstraint(*mShockAbsorberBodyMount, *mShockAbsorberWheelMount, ta, tb, true);
	mDW->addConstraint(mShockAbsorber, true);

	mShockAbsorber->setLimit(0,0,0);
	mShockAbsorber->setLimit(1,0,0);
	mShockAbsorber->setLimit(2,-springLoad, springLoad);

	mShockAbsorber->setLimit(3,-1,1);
	mShockAbsorber->setLimit(4,-1,1);
	mShockAbsorber->setLimit(5,-1,1);

	mShockAbsorber->enableSpring(2, true);
	mShockAbsorber->setStiffness(2, springStiffness);
	mShockAbsorber->setDamping(2, springDamping);
	mShockAbsorber->setEquilibriumPoint(2, springLoad);
	mShockAbsorber->setEnabled(true);
}

void WheelAssembly::ApplyParameters(WheelPairParams *p)
{
	float wheelRadius = p->WheelRadius;
	float wheelWidth = p->WheelWidth;
	float wheelMass = p->WheelMass;
	float wheelFriction = p->WheelFriction;
	float wheelLinearDamping = p->WheelLinearDamping;
	float wheelAngularDamping = p->WheelAngularDamping;
	float wheelRestitution = p->WheelRestitution;
	float axleHeight = p->AxleHeight;
	float axleWidth = p->AxleWidth;
	float axleMass = p->AxleMass;
	float axleLinearDamping = p->AxleLinearDamping;
	float axleAngularDamping = p->AxleAngularDamping;
	float wishBoneMass = p->WishBoneMass;
	float wishBoneWidth = p->WishBoneWidth;
	float wishBoneHeight = p->WishBoneHeight;
	float steeringArmMass = p->HubMass;
	float steeringArmHeight = p->HubHeight;
	float wishBoneLinearDamping = p->WishBoneLinearDamping;
	float wishBoneAngularDamping = p->WishBoneAngularDamping;
	float lowerBodyWishBoneZOffset = p->WishBoneLowerOffset;
	float upperBodyWishBoneZOffset = p->WishBoneUpperOffset;
	float springLength = p->SpringLength;
	float springStiffness = p->SpringStiffness;
	float springDamping = p->SpringDamping;
	float springLoad = p->SpringLoad;
	float angle = p->SpringAngle;	
	float axleLength = wheelWidth;
	float steeringArmWidth = wishBoneWidth;
	float steeringArmLength = wishBoneWidth;
	float upperWishBoneZWheelOffset = steeringArmHeight;
	float lowerWishBoneZWheelOffset = -steeringArmHeight;
	float upperWishBoneZBodyOffset = upperWishBoneZWheelOffset + upperBodyWishBoneZOffset;
	float lowerWishBoneZBodyOffset = lowerWishBoneZWheelOffset + lowerBodyWishBoneZOffset;

	float lim = SIMD_PI / 4;
	float lo = -lim;
	float hi = lim;

	mWheel->setFriction(wheelFriction);
	mWheel->setRestitution(wheelRestitution);
	mWheel->setActivationState(DISABLE_DEACTIVATION);

	mWheel->setDamping(wheelLinearDamping, wheelAngularDamping);
	mWheelAxle->setDamping(axleLinearDamping, axleAngularDamping);
	mUpperWishBone->setDamping(wishBoneLinearDamping, wishBoneAngularDamping);
	mLowerWishBone->setDamping(wishBoneLinearDamping, wishBoneAngularDamping);

	mLowerWishBoneWheelFrontHinge->setLimit(lo, hi);
	mUpperWishBoneWheelFrontHinge->setLimit(lo, hi);
	mLowerWishBoneBodyFrontHinge->setLimit(lo, hi);
	mUpperWishBoneBodyFrontHinge->setLimit(lo, hi);

	mLowerWishBoneWheelRearHinge->setLimit(lo, hi);
	mUpperWishBoneWheelRearHinge->setLimit(lo, hi);
	mLowerWishBoneBodyRearHinge->setLimit(lo, hi);
	mUpperWishBoneBodyRearHinge->setLimit(lo, hi);

	mShockAbsorber->setLimit(0,0,0);
	mShockAbsorber->setLimit(1,0,0);
	mShockAbsorber->setLimit(2,-springLoad, springLoad);

	mShockAbsorber->setLimit(3,-1,1);
	mShockAbsorber->setLimit(4,-1,1);
	mShockAbsorber->setLimit(5,-1,1);

	mShockAbsorber->enableSpring(2, true);
	mShockAbsorber->setStiffness(2, springStiffness);
	mShockAbsorber->setDamping(2, springDamping);
	mShockAbsorber->setEquilibriumPoint(2, springLoad);
	mShockAbsorber->setEnabled(true);
}

//////////////////////////////////////////////////////////////////////
// how much is this wheel sliding sideways (0 to 1)

float WheelAssembly::LateralSlip()
{
	btVector3 velocity = mWheel->getLinearVelocity();
	if(velocity.length2() > 0)
	{
		btVector3 y = mWheel->getWorldTransform().getBasis().getColumn(1);
		return fabsf(velocity.normalized().dot(y));
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
// -ive = skidding, +ive = wheels spinning

float WheelAssembly::Skid()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////

void WheelAssembly::Destroy()
{
	DeleteConstraint(mAxleHingeInner);
	DeleteConstraint(mAxleHingeOuter);
	DeleteConstraint(mLowerWishBoneWheelFrontHinge);
	DeleteConstraint(mUpperWishBoneWheelFrontHinge);
	DeleteConstraint(mLowerWishBoneBodyFrontHinge);
	DeleteConstraint(mUpperWishBoneBodyFrontHinge);
	DeleteConstraint(mLowerWishBoneWheelRearHinge);
	DeleteConstraint(mUpperWishBoneWheelRearHinge);
	DeleteConstraint(mLowerWishBoneBodyRearHinge);
	DeleteConstraint(mUpperWishBoneBodyRearHinge);
	DeleteConstraint(mShockAbsorberBodyMountHinge);
	DeleteConstraint(mShockAbsorberWheelMountHinge);
	DeleteConstraint(mShockAbsorber);
	DeleteConstraint(mSteeringHingeUpper);
	DeleteConstraint(mSteeringHingeLower);

	DeleteRigidBody(mWheel);
	DeleteRigidBody(mWheelAxle);
	DeleteRigidBody(mHub);
	DeleteRigidBody(mLowerWishBone);
	DeleteRigidBody(mUpperWishBone);
	DeleteRigidBody(mShockAbsorberBodyMount);
	DeleteRigidBody(mShockAbsorberWheelMount);

	Delete(mHubShape);
	Delete(mWheelShape);
	Delete(mWheelAxleShape);
	Delete(mLowerWishBoneShape);
	Delete(mUpperWishBoneShape);
	Delete(mSpringMountShape);
}

//////////////////////////////////////////////////////////////////////

WheelAssembly::~WheelAssembly()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////

Car::~Car()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////

void Car::Destroy()
{
	for(int i=0; i<NumWheels; ++i)
	{
		mWheelAssembly[i]->Destroy();
	}
	DeleteRigidBody(mBody);
	Delete(mBody);
	Delete(mBodyShape);
}

//////////////////////////////////////////////////////////////////////

void SetMotor(btHingeConstraint *hinge, bool enable, float maxForce, float targetVelocity)
{
	hinge->enableAngularMotor(true, targetVelocity, maxForce);
}

//////////////////////////////////////////////////////////////////////

void Car::ApplyPower(float speed, float force)
{
	SetMotor(mWheelAssembly[BackRight]->mAxleHingeInner, true, force, speed);
	SetMotor(mWheelAssembly[BackLeft]->mAxleHingeInner, true, force, speed);
	SetMotor(mWheelAssembly[BackRight]->mAxleHingeOuter, true, force, speed);
	SetMotor(mWheelAssembly[BackLeft]->mAxleHingeOuter, true, force, speed);
}

//////////////////////////////////////////////////////////////////////

void Car::AntiRoll()
{
	ApplyAntiRoll(mWheelAssembly[FrontLeft], mWheelAssembly[FrontRight]);
	ApplyAntiRoll(mWheelAssembly[BackLeft], mWheelAssembly[BackRight]);
}

//////////////////////////////////////////////////////////////////////

void Car::ApplyAntiRoll(WheelAssembly *left, WheelAssembly *right)
{
	//btVector3 leftSpringDir(left->mShockAbsorberBodyMount->getWorldTransform().getOrigin() - left->mShockAbsorberWheelMount->getWorldTransform().getOrigin());
	//btVector3 rightSpringDir(right->mShockAbsorberBodyMount->getWorldTransform().getOrigin() - right->mShockAbsorberWheelMount->getWorldTransform().getOrigin());

	//float leftSpringLength = leftSpringDir.length2();
	//float rightSpringLength = rightSpringDir.length2();

	//float force = 200;
	//float diff = leftSpringLength - rightSpringLength;
	//if(diff != 0)
	//{
	//	rightSpringDir.normalize();
	//	leftSpringDir.normalize();
	//	left->mShockAbsorberWheelMount->applyCentralForce(rightSpringDir * diff * force);
	//	left->mShockAbsorberBodyMount->applyCentralForce(rightSpringDir * -diff * force);
	//	right->mShockAbsorberWheelMount->applyCentralForce(leftSpringDir * -diff * force);
	//	right->mShockAbsorberBodyMount->applyCentralForce(leftSpringDir * diff * force);
	//}
}

