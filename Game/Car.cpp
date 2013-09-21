//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Car.h"

//////////////////////////////////////////////////////////////////////

vector<ParameterSet *> ParameterSet::mSets;			// all the parameter sets
vector<Parameter *> ParameterSet::mParameterList;		// TEMP! real ones are stored in the set, this is a placeholder filled in during construction
int Parameter::numParameters = 0;

//////////////////////////////////////////////////////////////////////

char const *test =
"<xml>\n\
	<ParameterSet Name=\"Car\" ParameterCount=\"4\">\n\
		<Parameter Name=\"bodyMass\" Value=\"70.000000\" MinValue=\"1.000000\" MaxValue=\"500.000000\" DefaultValue=\"70.000000\" />\n\
		<Parameter Name=\"bodyLength\" Value=\"20.000000\" MinValue=\"10.000000\" MaxValue=\"30.000000\" DefaultValue=\"20.000000\" />\n\
		<Parameter Name=\"bodyWidth\" Value=\"4.000000\" MinValue=\"0.500000\" MaxValue=\"10.000000\" DefaultValue=\"4.000000\" />\n\
		<Parameter Name=\"bodyHeight\" Value=\"3.000000\" MinValue=\"0.500000\" MaxValue=\"5.000000\" DefaultValue=\"3.000000\" />\n\
	</ParameterSet>\n\
	<ParameterSet Name=\"Rear Wheels\" ParameterCount=\"30\">\n\
		<Parameter Name=\"WheelMass\" Value=\"1.000000\" MinValue=\"0.100000\" MaxValue=\"50.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"WheelFriction\" Value=\"1.200000\" MinValue=\"0.500000\" MaxValue=\"16.000000\" DefaultValue=\"1.200000\" />\n\
		<Parameter Name=\"WheelLinearDamping\" Value=\"0.600000\" MinValue=\"0.000000\" MaxValue=\"5.000000\" DefaultValue=\"0.600000\" />\n\
		<Parameter Name=\"WheelAngularDamping\" Value=\"0.500000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.500000\" />\n\
		<Parameter Name=\"WheelRestitution\" Value=\"0.000000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.000000\" />\n\
		<Parameter Name=\"AxleMass\" Value=\"4.000000\" MinValue=\"1.000000\" MaxValue=\"50.000000\" DefaultValue=\"4.000000\" />\n\
		<Parameter Name=\"AxleLinearDamping\" Value=\"0.100000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.100000\" />\n\
		<Parameter Name=\"AxleAngularDamping\" Value=\"1.000000\" MinValue=\"0.000000\" MaxValue=\"2.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"WishBoneMass\" Value=\"4.000000\" MinValue=\"1.000000\" MaxValue=\"50.000000\" DefaultValue=\"4.000000\" />\n\
		<Parameter Name=\"SteeringArmMass\" Value=\"8.000000\" MinValue=\"1.000000\" MaxValue=\"16.000000\" DefaultValue=\"8.000000\" />\n\
		<Parameter Name=\"WishBoneLinearDamping\" Value=\"0.250000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.250000\" />\n\
		<Parameter Name=\"WishBoneAngularDamping\" Value=\"0.50000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.50000\" />\n\
		<Parameter Name=\"SpringStiffness\" Value=\"1200.000000\" MinValue=\"10.000000\" MaxValue=\"2000.000000\" DefaultValue=\"1200.000000\" />\n\
		<Parameter Name=\"SpringDamping\" Value=\"0.5000\" MinValue=\"0.0001\" MaxValue=\"1.000000\" DefaultValue=\"0.5000\" />\n\
		<Parameter Name=\"SpringLoad\" Value=\"0.750000\" MinValue=\"0.150000\" MaxValue=\"4.500000\" DefaultValue=\"0.750000\" />\n\
		<Parameter Name=\"WheelOffsetX\" Value=\"2.000000\" MinValue=\"-4.000000\" MaxValue=\"4.000000\" DefaultValue=\"2.000000\" />\n\
		<Parameter Name=\"WheelOffsetY\" Value=\"5.500000\" MinValue=\"2.500000\" MaxValue=\"10.000000\" DefaultValue=\"5.500000\" />\n\
		<Parameter Name=\"WheelRadius\" Value=\"2.500000\" MinValue=\"2.000000\" MaxValue=\"5.000000\" DefaultValue=\"2.500000\" />\n\
		<Parameter Name=\"WheelWidth\" Value=\"1.000000\" MinValue=\"0.150000\" MaxValue=\"4.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"AxleHeight\" Value=\"0.250000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"0.250000\" />\n\
		<Parameter Name=\"AxleWidth\" Value=\"0.250000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"0.250000\" />\n\
		<Parameter Name=\"WishBoneWidth\" Value=\"0.600000\" MinValue=\"0.100000\" MaxValue=\"1.500000\" DefaultValue=\"0.600000\" />\n\
		<Parameter Name=\"WishBoneHeight\" Value=\"0.020000\" MinValue=\"0.010000\" MaxValue=\"0.200000\" DefaultValue=\"0.020000\" />\n\
		<Parameter Name=\"SteeringArmWidth\" Value=\"1.000000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"SteeringArmHeight\" Value=\"1.000000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"SteeringArmLength\" Value=\"1.000000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"WishBoneLowerOffset\" Value=\"0.250000\" MinValue=\"-1.000000\" MaxValue=\"2.000000\" DefaultValue=\"0.250000\" />\n\
		<Parameter Name=\"WishBoneUpperOffset\" Value=\"1.250000\" MinValue=\"-0.250000\" MaxValue=\"2.250000\" DefaultValue=\"1.250000\" />\n\
		<Parameter Name=\"SpringLength\" Value=\"5.000000\" MinValue=\"2.000000\" MaxValue=\"10.000000\" DefaultValue=\"5.000000\" />\n\
		<Parameter Name=\"SpringAngle\" Value=\"0.436332\" MinValue=\"-0.436332\" MaxValue=\"1.047198\" DefaultValue=\"0.436332\" />\n\
	</ParameterSet>\n\
	<ParameterSet Name=\"Front Wheels\" ParameterCount=\"30\">\n\
		<Parameter Name=\"WheelMass\" Value=\"1.000000\" MinValue=\"0.100000\" MaxValue=\"50.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"WheelFriction\" Value=\"1.200000\" MinValue=\"0.500000\" MaxValue=\"16.000000\" DefaultValue=\"1.200000\" />\n\
		<Parameter Name=\"WheelLinearDamping\" Value=\"0.600000\" MinValue=\"0.000000\" MaxValue=\"5.000000\" DefaultValue=\"0.600000\" />\n\
		<Parameter Name=\"WheelAngularDamping\" Value=\"0.500000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.500000\" />\n\
		<Parameter Name=\"WheelRestitution\" Value=\"0.000000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.000000\" />\n\
		<Parameter Name=\"AxleMass\" Value=\"4.000000\" MinValue=\"1.000000\" MaxValue=\"50.000000\" DefaultValue=\"4.000000\" />\n\
		<Parameter Name=\"AxleLinearDamping\" Value=\"0.100000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.100000\" />\n\
		<Parameter Name=\"AxleAngularDamping\" Value=\"0.500000\" MinValue=\"0.000000\" MaxValue=\"2.000000\" DefaultValue=\"0.500000\" />\n\
		<Parameter Name=\"WishBoneMass\" Value=\"4.000000\" MinValue=\"1.000000\" MaxValue=\"50.000000\" DefaultValue=\"4.000000\" />\n\
		<Parameter Name=\"SteeringArmMass\" Value=\"8.000000\" MinValue=\"1.000000\" MaxValue=\"16.000000\" DefaultValue=\"8.000000\" />\n\
		<Parameter Name=\"WishBoneLinearDamping\" Value=\"0.250000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.250000\" />\n\
		<Parameter Name=\"WishBoneAngularDamping\" Value=\"0.50000\" MinValue=\"0.000000\" MaxValue=\"1.000000\" DefaultValue=\"0.50000\" />\n\
		<Parameter Name=\"SpringStiffness\" Value=\"1200.000000\" MinValue=\"10.000000\" MaxValue=\"2000.000000\" DefaultValue=\"1200.000000\" />\n\
		<Parameter Name=\"SpringDamping\" Value=\"0.5000\" MinValue=\"0.0001\" MaxValue=\"1.000000\" DefaultValue=\"0.5000\" />\n\
		<Parameter Name=\"SpringLoad\" Value=\"0.750000\" MinValue=\"0.150000\" MaxValue=\"4.500000\" DefaultValue=\"0.750000\" />\n\
		<Parameter Name=\"WheelOffsetX\" Value=\"2.000000\" MinValue=\"-4.000000\" MaxValue=\"4.000000\" DefaultValue=\"2.000000\" />\n\
		<Parameter Name=\"WheelOffsetY\" Value=\"5.500000\" MinValue=\"2.500000\" MaxValue=\"10.000000\" DefaultValue=\"5.500000\" />\n\
		<Parameter Name=\"WheelRadius\" Value=\"2.500000\" MinValue=\"2.000000\" MaxValue=\"5.000000\" DefaultValue=\"2.500000\" />\n\
		<Parameter Name=\"WheelWidth\" Value=\"1.000000\" MinValue=\"0.150000\" MaxValue=\"4.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"AxleHeight\" Value=\"0.250000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"0.250000\" />\n\
		<Parameter Name=\"AxleWidth\" Value=\"0.250000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"0.250000\" />\n\
		<Parameter Name=\"WishBoneWidth\" Value=\"0.600000\" MinValue=\"0.100000\" MaxValue=\"1.500000\" DefaultValue=\"0.600000\" />\n\
		<Parameter Name=\"WishBoneHeight\" Value=\"0.020000\" MinValue=\"0.010000\" MaxValue=\"0.200000\" DefaultValue=\"0.020000\" />\n\
		<Parameter Name=\"SteeringArmWidth\" Value=\"1.000000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"SteeringArmHeight\" Value=\"1.000000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"SteeringArmLength\" Value=\"1.000000\" MinValue=\"0.100000\" MaxValue=\"2.000000\" DefaultValue=\"1.000000\" />\n\
		<Parameter Name=\"WishBoneLowerOffset\" Value=\"0.250000\" MinValue=\"-1.000000\" MaxValue=\"2.000000\" DefaultValue=\"0.250000\" />\n\
		<Parameter Name=\"WishBoneUpperOffset\" Value=\"1.250000\" MinValue=\"-0.250000\" MaxValue=\"2.250000\" DefaultValue=\"1.250000\" />\n\
		<Parameter Name=\"SpringLength\" Value=\"5.000000\" MinValue=\"2.000000\" MaxValue=\"10.000000\" DefaultValue=\"5.000000\" />\n\
		<Parameter Name=\"SpringAngle\" Value=\"0.436332\" MinValue=\"-0.436332\" MaxValue=\"1.047198\" DefaultValue=\"0.436332\" />\n\
	</ParameterSet>\n\
</xml>\n";
void ParameterSet::SetFromXML(XmlDocument *xmlDocument, char const *pname)
{
	XmlNode *root = xmlDocument->first_node("xml");
	if(root != null)
	{
		XmlNode *node = root->first_node("ParameterSet");
		while(node != null)
		{
			XmlAttribute *name = node->first_attribute("Name");
			XmlAttribute *count = node->first_attribute("ParameterCount");
			if(name != null && count != null)
			{
				if(strcmp(pname, name->value()) == 0)
				{
					mName = string(name->value());
					int paramCount = atoi(count->value());
					if(paramCount > 0)
					{
						int found = 0;
						XmlNode *param = node->first_node("Parameter");
						while(param != null)
						{
							XmlAttribute *name = param->first_attribute("Name");
							XmlAttribute *value = param->first_attribute("Value");
							XmlAttribute *minValue = param->first_attribute("MinValue");
							XmlAttribute *maxValue = param->first_attribute("MaxValue");
							XmlAttribute *defaultValue = param->first_attribute("DefaultValue");

							if(name != null && value != null && minValue != null && maxValue != null && defaultValue != null)
							{
								for(auto p: mParameters)
								{
									if(p->strName.compare(name->value()) == 0)
									{
										p->mValue = (float)atof(value->value());
										p->minValue = (float)atof(minValue->value());
										p->maxValue = (float)atof(maxValue->value());
										p->defaultValue = (float)atof(defaultValue->value());
										break;
									}
								}
								TRACE("%s = %f\n", name->value(), (float)atof(value->value()));
								++found;
								if(found > paramCount)
								{
									// error
								}
							}
							param = param->next_sibling();
						}
					}
					break;
				}
			}
			node = node->next_sibling();
		}
	}
}

//////////////////////////////////////////////////////////////////////

void Car::SetParametersFromXML(char *fileContents)
{
	XmlDocument xml;
	try
	{
		xml.parse<rapidxml::parse_default>(fileContents);
		mCarParams.SetFromXML(&xml, "Car");
		mRearWheelParams.SetFromXML(&xml, "Rear Wheels");
		mFrontWheelParams.SetFromXML(&xml, "Front Wheels");
	}
	catch(rapidxml::parse_error e)
	{
		TRACE("Error parsing XML: %s\n", e.what());
	}
}

//////////////////////////////////////////////////////////////////////

Car::Car(btDynamicsWorld *dynamicsWorld)
	: PhysicalObject(dynamicsWorld)
	, mBody(null)
	, mBodyShape(null)
	, mCarParams("Car")
	, mFrontWheelParams("Front Wheels")
	, mRearWheelParams("Rear Wheels")
{
	mWheelAssembly[BackRight] = new WheelAssembly(mDW, true, true, *this);
	mWheelAssembly[BackLeft] = new WheelAssembly(mDW, false, true, *this);
	mWheelAssembly[FrontRight] = new WheelAssembly(mDW, true, false, *this);
	mWheelAssembly[FrontLeft] = new WheelAssembly(mDW, false, false, *this);

	////TRACE("%s", ToXMLString().c_str());
	//size_t s = strlen(test);
	//char *p = new char[s + 1];
	//memcpy(p, test, s);
	//p[s] = 0;
	//SetParametersFromXML(p);
	//delete[] p;
	//TRACE("%s\n", ToXMLString().c_str());
}

//////////////////////////////////////////////////////////////////////

string Car::ToXMLString() const
{
	string s("<xml>\n");
	s += mCarParams.ToXMLString();
	s += mRearWheelParams.ToXMLString();
	s += mFrontWheelParams.ToXMLString();
	s += "</xml>\n";
	return s;
}

//////////////////////////////////////////////////////////////////////

WheelAssembly::WheelAssembly(btDynamicsWorld *dynamicsWorld, bool isRightWheel, bool isRearWheel, Car &car)
	: PhysicalObject(dynamicsWorld)
	, mAxleHingeInner(null)
	, mAxleHingeOuter(null)
	, mSteeringArm(null)
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
	, mSteeringArmShape(null)
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
	float steeringArmMass = p->SteeringArmMass;
	float steeringArmHeight = p->SteeringArmHeight;
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
	mSteeringArmShape = new btBoxShape(btVector3(steeringArmWidth, steeringArmLength, steeringArmHeight));
	btTransform steeringArmTransform(btQuaternion::getIdentity(), wheelPos);
	btDefaultMotionState *steeringArmMotionState = new btDefaultMotionState(steeringArmTransform);
	mSteeringArm = new btRigidBody(steeringArmMass, steeringArmMotionState, mSteeringArmShape, inertia(steeringArmMass, mSteeringArmShape));
	mDW->addRigidBody(mSteeringArm, 0, 0);

	// join the steering arm to the axle
	mSteeringHingeUpper = new btHingeConstraint(*mWheelAxle, *mSteeringArm, btVector3(0,0,steeringArmHeight), btVector3(0, 0, steeringArmHeight), btVector3(0,0,1), btVector3(0,0,1), true);
	mSteeringHingeLower = new btHingeConstraint(*mWheelAxle, *mSteeringArm, btVector3(0,0,-steeringArmHeight), btVector3(0, 0, -steeringArmHeight), btVector3(0,0,1), btVector3(0,0,1), true);

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

		float lowerWishBoneLength = fabsf(lowerArmDelta.length()) / 2;
		float upperWishBoneLength = fabsf(upperArmDelta.length()) / 2;

		float lowerWishBoneAngle = asinf(lowerArmDelta.z() / lowerWishBoneLength) * 0.5f * yReflect;
		float upperWishBoneAngle = asinf(upperArmDelta.z() / upperWishBoneLength) * 0.5f * yReflect;

		btVector3 lowerArmOrigin = (armJoinPos + lowerBodyJoinPosition) / 2;
		btVector3 upperArmOrigin = (armJoinPos + upperBodyJoinPosition) / 2;

		// Create the lower wishbones
		mLowerWishBoneShape = new btBoxShape(btVector3(wishBoneWidth, lowerWishBoneLength, wishBoneHeight));
		mUpperWishBoneShape = new btBoxShape(btVector3(wishBoneWidth, upperWishBoneLength, wishBoneHeight));

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

		mLowerWishBoneWheelFrontHinge = new btHingeConstraint(*mSteeringArm, *mLowerWishBone, lowerArmFrontJoinPos, btVector3(wishBoneWidth, lowerWishBoneLength * -yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mLowerWishBoneBodyFrontHinge = new btHingeConstraint(*mCar->mBody, *mLowerWishBone, lowerBodyFrontJointPos, btVector3(wishBoneWidth, lowerWishBoneLength * yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mLowerWishBoneWheelRearHinge = new btHingeConstraint(*mSteeringArm, *mLowerWishBone, lowerArmRearJoinPos, btVector3(-wishBoneWidth, lowerWishBoneLength * -yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mLowerWishBoneBodyRearHinge = new btHingeConstraint(*mCar->mBody, *mLowerWishBone, lowerBodyRearJointPos, btVector3(-wishBoneWidth, lowerWishBoneLength * yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);

		mUpperWishBoneWheelFrontHinge = new btHingeConstraint(*mSteeringArm, *mUpperWishBone, upperArmFrontJoinPos, btVector3(wishBoneWidth, upperWishBoneLength * -yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mUpperWishBoneBodyFrontHinge = new btHingeConstraint(*mCar->mBody, *mUpperWishBone, upperBodyFrontJointPos, btVector3(wishBoneWidth, upperWishBoneLength * yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mUpperWishBoneWheelRearHinge = new btHingeConstraint(*mSteeringArm, *mUpperWishBone, upperArmRearJoinPos, btVector3(-wishBoneWidth, upperWishBoneLength * -yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);
		mUpperWishBoneBodyRearHinge = new btHingeConstraint(*mCar->mBody, *mUpperWishBone, upperBodyRearJointPos, btVector3(-wishBoneWidth, upperWishBoneLength * yReflect, 0), btVector3(1, 0, 0), btVector3(1, 0, 0), true);

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
	mShockAbsorberWheelMountHinge = new btPoint2PointConstraint(*mSteeringArm, *mShockAbsorberWheelMount, springOffset, btVector3(0,0,0));
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
	float steeringArmMass = p->SteeringArmMass;
	float steeringArmHeight = p->SteeringArmHeight;
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
	DeleteRigidBody(mSteeringArm);
	DeleteRigidBody(mLowerWishBone);
	DeleteRigidBody(mUpperWishBone);
	DeleteRigidBody(mShockAbsorberBodyMount);
	DeleteRigidBody(mShockAbsorberWheelMount);

	Delete(mSteeringArmShape);
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

