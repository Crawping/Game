//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

struct aiScene;
struct aiNode;

#include "Car.h"

class MyApp : App
{
	void		OnInit() override;
	bool		OnUpdate() override;
	void		OnClose() override;
	void		InitPhysics();
	void		UpdatePhysics();
	void		DrawPhysics();
	void		CleanUpPhysics();
	void		UpdateCamera();
	void		Draw2DAxes(int x, int y, int w, int h);
	void		OrthoDraw(int axis, int upAxis, int l, int t, int r, int b);
	void		CountParameters();
	void		DrawParameters();
	void		Draw();
	void		DumpNode(aiScene const *scene, aiNode const *node, int indent = 0);
	void		DrawNode(aiScene const *scene, aiNode const *node);
	void		CreateRamp();
	void		DeleteRamp();

	uint									mFrame;
	Timer									mTimer;
	double									mElapsedTime;
	float									mDeltaTime;
	Camera									mCamera;

	Vec3									mPosition;
	Vec3									mTarget;
	float									mYaw;
	float									mPitch;
	float									mRoll;

	Model *									mModel;

	Model *									mCylinder;
	Mesh *									mCylinderMesh;
	VertexBuffer *							mCylinderVB;
	IndexBuffer *							mCylinderIB;

	Console	*								mConsole;
	ImmediateContext *						mImmediateContext;
	SpriteList *							mSpriteList;
	VertexShader *							mVertexShader;
	PixelShader *							mPixelShader;
	Texture *								mTexture;
	Material *								mMaterial;
	VertexBuffer *							mVertexBuffer;
	IndexBuffer *							mIndexBuffer;
	ConstantBuffer *						mConstantBuffer;
	Mesh *									mMesh;

	Axes *									mAxes;

	VertexShader *							m2DUntexturedVS;
	PixelShader *							m2DUntexturedPS;
	Material *								mUntexturedMaterial;
	ImmediateContext *						m2DUntexturedIC;

	int										mTransformConstantBufferIndex;

	Font *									mDebugFont;
	Font *									mBannerFont;
	Font *									mFixedSysFont;

	PhysicsDebug *							mPhysicsDebug;

	float									mSteering;
	Vec3									mCameraPos;
	float									mCameraDistance;
	float									mCameraHeight;
	float									mCameraTargetHeight;

	Vec2									mOldMouseDelta;
	float									mCameraYaw;
	float									mCameraPitch;

	Car *									mCar;
	uint32									mEditMode;

	int										mParameterIndex;
	float									mScrollBarPos;
	float									mScrollBarVelocity;
	int										mNumParameterLines;

	btDefaultCollisionConfiguration *		collisionConfiguration;
	btCollisionDispatcher *					dispatcher;
	btBroadphaseInterface *					overlappingPairCache;
	btSequentialImpulseConstraintSolver *	solver;
	btDiscreteDynamicsWorld *				dynamicsWorld;
	btCollisionShape *						groundShape;
	btRigidBody *							groundRigidBody;

	btCollisionShape *						mRampShape;
	btRigidBody *							mRamp;

	btAlignedObjectArray<btCollisionShape*>	collisionShapes;
};

//////////////////////////////////////////////////////////////////////

