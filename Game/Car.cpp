//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Car.h"

//////////////////////////////////////////////////////////////////////

Car::Car()
	: mBody(null)
	, mBodyShape(null)
	, mEngineShape(null)
	, mBodyAndEngineShape(null)
	, mRearWheelParams("Rear Wheels")
	, mFrontWheelParams("Front Wheels")
	, mParameterSets(_T("car.xml"))
{
	mParameterSets.AddParameterSet(&mEngineParams);
	mParameterSets.AddParameterSet(&mCarParams);
	mParameterSets.AddParameterSet(&mRearWheelParams);
	mParameterSets.AddParameterSet(&mFrontWheelParams);

	mParameterSets.Load();

	mWheelAssembly[BackRight] = new WheelAssembly(true, true, *this);
	mWheelAssembly[BackLeft] = new WheelAssembly(false, true, *this);
	mWheelAssembly[FrontRight] = new WheelAssembly(true, false, *this);
	mWheelAssembly[FrontLeft] = new WheelAssembly(false, false, *this);
}

//////////////////////////////////////////////////////////////////////

WheelAssembly::WheelAssembly(bool isRightWheel, bool isRearWheel, Car &car)
	: mAxleHingeInner(null)
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

void Car::Create()
{
	Destroy();

	Vector carPos = Vec(0, 0, 5);
	Vector carSize = Vec(mCarParams.Length, mCarParams.Width, mCarParams.Height);
	float bodyMass = mCarParams.Mass;

	btTransform bodyTransform(btQuaternion::getIdentity(), carPos);

	mBodyShape = new btBoxShape(carSize);

#if ENGINE
	//mEngineShape = new btBoxShape(btVector3(mEngineParams.Length, mEngineParams.Width, mEngineParams.Height));

	//mBodyAndEngineShape = new btCompoundShape();

	//Vec3 ep(carPos + Vec3(mEngineParams.HorizontalOffset, 0, mEngineParams.VerticalOffset));

	//btTransform engineTransform(btQuaternion::getIdentity(), btVector3(ep.x, ep.y, ep.z));

	//mBodyAndEngineShape->addChildShape(bodyTransform, mBodyShape);
	//mBodyAndEngineShape->addChildShape(engineTransform, mEngineShape);

	//float engineMass = mEngineParams.Mass;

	//float totalMass = engineMass + bodyMass;

	//float md = 1.0f / totalMass;

	//float masses[2] = { bodyMass * md, engineMass * md };

	//btTransform shift;
	//shift.setIdentity();

	//btVector3 localInertia(0,0,0);

	//btCompoundShape *newShape = Physics::InitCompoundShape(mBodyAndEngineShape, masses, shift);
	//Delete(mBodyAndEngineShape);
	//mBodyAndEngineShape = newShape;

	//mBodyAndEngineShape->calculateLocalInertia(totalMass, localInertia);

	//mBody = new btRigidBody(totalMass, new btDefaultMotionState(bodyTransform * shift), mBodyAndEngineShape, localInertia);
#else
	mBody = new btRigidBody(bodyMass, new btDefaultMotionState(bodyTransform), mBodyShape, Physics::inertia(bodyMass, mBodyShape));
#endif
	Physics::DynamicsWorld->addRigidBody(mBody, 4, 1);

	btVector3 cpos = carPos;
	btVector3 csize = carSize;

	mWheelAssembly[FrontLeft]->Create(&mFrontWheelParams, cpos, csize);
	mWheelAssembly[FrontRight]->Create(&mFrontWheelParams, cpos, csize);
	mWheelAssembly[BackLeft]->Create(&mRearWheelParams, cpos, csize);
	mWheelAssembly[BackRight]->Create(&mRearWheelParams, cpos, csize);
	ApplyParameters();
}

//////////////////////////////////////////////////////////////////////

void Car::ApplyParameters()
{
	mBody->setMassProps(mCarParams.Mass, Physics::inertia(mCarParams.Mass, mBodyShape));

	mWheelAssembly[FrontLeft]->ApplyParameters(&mFrontWheelParams);
	mWheelAssembly[FrontRight]->ApplyParameters(&mFrontWheelParams);
	mWheelAssembly[BackLeft]->ApplyParameters(&mRearWheelParams);
	mWheelAssembly[BackRight]->ApplyParameters(&mRearWheelParams);
}

//////////////////////////////////////////////////////////////////////

void WheelAssembly::Create(WheelPairParams *p, btVector3 const &carWorldPos, btVector3 const &carSize)
{
	float xReflect = mIsRearWheel ? 1.0f : -1.0f;
	float yReflect = mIsRightWheel ? 1.0f : -1.0f;

	float carLength = carSize.x();
	float carWidth = carSize.y();
	float carHeight = carSize.z();

	Vector wheelOffset = Vec((carLength - p->Offset_X) * xReflect, (carWidth + p->Offset_Y) * -yReflect, 0);
	btVector3 wheelPos = wheelOffset + carWorldPos;
	float wheelRadius = p->Radius;
	float wheelWidth = p->Width;
	float wheelMass = p->Mass;
	float wheelFriction = p->Friction;
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
	mWheel = new btRigidBody(wheelMass, wheelMotionState, mWheelShape, Physics::inertia(wheelMass, mWheelShape));
	Physics::DynamicsWorld->addRigidBody(mWheel, 2, 1);
	mWheel->setDamping(wheelLinearDamping, wheelAngularDamping);
	mWheel->setFriction(wheelFriction);
	mWheel->setRestitution(wheelRestitution);
	mWheel->setActivationState(DISABLE_DEACTIVATION);

	// Create the axle
	mWheelAxleShape = new btBoxShape(btVector3(axleWidth, axleLength, axleHeight));
	btTransform hubTransform(btQuaternion::getIdentity(), wheelPos);
	btDefaultMotionState *hubMotionState = new btDefaultMotionState(hubTransform);
	mWheelAxle = new btRigidBody(axleMass, hubMotionState, mWheelAxleShape, Physics::inertia(axleMass, mWheelAxleShape));
	Physics::DynamicsWorld->addRigidBody(mWheelAxle, 0, 0);
	mWheelAxle->setDamping(axleLinearDamping, axleAngularDamping);

	// Create hinges joining wheel to hub
	mAxleHingeInner = new btHingeConstraint(*mWheelAxle, *mWheel, btVector3(0, wheelWidth, 0), btVector3(0, wheelWidth, 0), btVector3(0,-1,0), btVector3(0,-1,0), true);
	mAxleHingeOuter = new btHingeConstraint(*mWheelAxle, *mWheel, btVector3(0, -wheelWidth, 0), btVector3(0, -wheelWidth, 0), btVector3(0,-1,0), btVector3(0,-1,0), true);

	Physics::DynamicsWorld->addConstraint(mAxleHingeInner, true);
	Physics::DynamicsWorld->addConstraint(mAxleHingeOuter, true);

	// create the steering arm
	mHubShape = new btBoxShape(btVector3(steeringArmLength, steeringArmWidth, steeringArmHeight));
	btTransform steeringArmTransform(btQuaternion::getIdentity(), wheelPos);
	btDefaultMotionState *steeringArmMotionState = new btDefaultMotionState(steeringArmTransform);
	mHub = new btRigidBody(steeringArmMass, steeringArmMotionState, mHubShape, Physics::inertia(steeringArmMass, mHubShape));
	Physics::DynamicsWorld->addRigidBody(mHub, 0, 0);

	// join the steering arm to the axle
	mSteeringHingeUpper = new btHingeConstraint(*mWheelAxle, *mHub, btVector3(0,0,steeringArmHeight), btVector3(0, 0, steeringArmHeight), btVector3(0,0,1), btVector3(0,0,1), true);
	mSteeringHingeLower = new btHingeConstraint(*mWheelAxle, *mHub, btVector3(0,0,-steeringArmHeight), btVector3(0, 0, -steeringArmHeight), btVector3(0,0,1), btVector3(0,0,1), true);

	Physics::DynamicsWorld->addConstraint(mSteeringHingeUpper, true);
	Physics::DynamicsWorld->addConstraint(mSteeringHingeLower, true);

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

		mLowerWishBone = new btRigidBody(wishBoneMass, lowerArmMotionState, mLowerWishBoneShape, Physics::inertia(wishBoneMass, mLowerWishBoneShape));
		mUpperWishBone = new btRigidBody(wishBoneMass, upperArmMotionState, mUpperWishBoneShape, Physics::inertia(wishBoneMass, mUpperWishBoneShape));

		Physics::DynamicsWorld->addRigidBody(mLowerWishBone, 0, 0);
		Physics::DynamicsWorld->addRigidBody(mUpperWishBone, 0, 0);

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

		Physics::DynamicsWorld->addConstraint(mLowerWishBoneWheelFrontHinge, true);
		Physics::DynamicsWorld->addConstraint(mUpperWishBoneWheelFrontHinge, true);
		Physics::DynamicsWorld->addConstraint(mLowerWishBoneBodyFrontHinge, true);
		Physics::DynamicsWorld->addConstraint(mUpperWishBoneBodyFrontHinge, true);

		Physics::DynamicsWorld->addConstraint(mLowerWishBoneWheelRearHinge, true);
		Physics::DynamicsWorld->addConstraint(mUpperWishBoneWheelRearHinge, true);
		Physics::DynamicsWorld->addConstraint(mLowerWishBoneBodyRearHinge, true);
		Physics::DynamicsWorld->addConstraint(mUpperWishBoneBodyRearHinge, true);

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
	mShockAbsorberBodyMount = new btRigidBody(springMountShapeMass, springMountBodyMotionState, mSpringMountShape, Physics::inertia(springMountShapeMass, mSpringMountShape));
	Physics::DynamicsWorld->addRigidBody(mShockAbsorberBodyMount, 0, 0);

	// Create Spring Mount on the axle
	btTransform springMountWheelTransform(btQuaternion::getIdentity(), armJoinPos);
	btDefaultMotionState *springMountWheelMotionState = new btDefaultMotionState(springMountWheelTransform);
	mShockAbsorberWheelMount = new btRigidBody(springMountShapeMass, springMountWheelMotionState, mSpringMountShape, Physics::inertia(springMountShapeMass, mSpringMountShape));
	Physics::DynamicsWorld->addRigidBody(mShockAbsorberWheelMount, 0, 0);

	// Attach Spring Mount to the body of the car with a free hinge
	mShockAbsorberBodyMountHinge = new btPoint2PointConstraint(*mCar->mBody, *mShockAbsorberBodyMount, springJoinPos, btVector3(0,0,0));
	Physics::DynamicsWorld->addConstraint(mShockAbsorberBodyMountHinge, true);

	// Attach Spring Mount to the steering arm with a free hinge
	mShockAbsorberWheelMountHinge = new btPoint2PointConstraint(*mHub, *mShockAbsorberWheelMount, springOffset, btVector3(0,0,0));
	Physics::DynamicsWorld->addConstraint(mShockAbsorberWheelMountHinge, true);

	// Attach the 2 Spring Mounts to each other with a spring
	btVector3 a = mShockAbsorberWheelMount->getWorldTransform().getOrigin();
	btVector3 b = mShockAbsorberBodyMount->getWorldTransform().getOrigin();
	btMatrix3x3 ma = GetOrientedBasis((a-b).normalize());
	mShockAbsorberBodyMount->getWorldTransform().setBasis(ma);
	mShockAbsorberWheelMount->getWorldTransform().setBasis(ma);

	btTransform ta(btQuaternion::getIdentity(), btVector3(0,0,springLength));
	btTransform tb(btQuaternion::getIdentity(), btVector3(0,0,0));
	mShockAbsorber = new btGeneric6DofSpringConstraint(*mShockAbsorberBodyMount, *mShockAbsorberWheelMount, ta, tb, true);
	Physics::DynamicsWorld->addConstraint(mShockAbsorber, true);

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
	float wheelRadius = p->Radius;
	float wheelWidth = p->Width;
	float wheelMass = p->Mass;
	float wheelFriction = p->Friction;
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
	Physics::DeleteConstraint(mAxleHingeInner);
	Physics::DeleteConstraint(mAxleHingeOuter);
	Physics::DeleteConstraint(mLowerWishBoneWheelFrontHinge);
	Physics::DeleteConstraint(mUpperWishBoneWheelFrontHinge);
	Physics::DeleteConstraint(mLowerWishBoneBodyFrontHinge);
	Physics::DeleteConstraint(mUpperWishBoneBodyFrontHinge);
	Physics::DeleteConstraint(mLowerWishBoneWheelRearHinge);
	Physics::DeleteConstraint(mUpperWishBoneWheelRearHinge);
	Physics::DeleteConstraint(mLowerWishBoneBodyRearHinge);
	Physics::DeleteConstraint(mUpperWishBoneBodyRearHinge);
	Physics::DeleteConstraint(mShockAbsorberBodyMountHinge);
	Physics::DeleteConstraint(mShockAbsorberWheelMountHinge);
	Physics::DeleteConstraint(mShockAbsorber);
	Physics::DeleteConstraint(mSteeringHingeUpper);
	Physics::DeleteConstraint(mSteeringHingeLower);

	Physics::DeleteRigidBody(mWheel);
	Physics::DeleteRigidBody(mWheelAxle);
	Physics::DeleteRigidBody(mHub);
	Physics::DeleteRigidBody(mLowerWishBone);
	Physics::DeleteRigidBody(mUpperWishBone);
	Physics::DeleteRigidBody(mShockAbsorberBodyMount);
	Physics::DeleteRigidBody(mShockAbsorberWheelMount);

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
	Physics::DeleteRigidBody(mBody);
	Delete(mBodyAndEngineShape);
	Delete(mEngineShape);
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

