//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

#include "linearmath/btIDebugDraw.h"

//////////////////////////////////////////////////////////////////////

class PhysicsDebug : public btIDebugDraw
{
public:

	PhysicsDebug();
	~PhysicsDebug();

	void Begin(Camera *camera, SpriteList *spriteList, Font *debugFont);
	void End();

	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location,const char* textString);
	void setDebugMode(int debugMode);
	int getDebugMode() const;
	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);

private:

	ImmediateContext *		mIC;
	Material *				mMaterial;
	VertexShader *			mVertexShader;
	PixelShader *			mPixelShader;
	Camera *				mCamera;
	Font *					mDebugFont;
	SpriteList *			mSpriteList;
	int						mDebugModes;
};

