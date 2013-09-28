//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

PhysicsDebug::PhysicsDebug()
{
	mDebugModes = 0;
	uint vmask = VertexElement::Position3 | VertexElement::Color;
	mVertexShader = VertexShader::Load(L"UntexturedVertexShader", vmask);
	mPixelShader = PixelShader::Load(L"UntexturedPixelShader");
	mMaterial = Material::Create(mPixelShader, mVertexShader, BM_None);
	mIC = ImmediateContext::Create(vmask, 65536, 65536);
}

//////////////////////////////////////////////////////////////////////

PhysicsDebug::~PhysicsDebug()
{
	::Release(mIC);
	::Release(mMaterial);
	::Release(mPixelShader);
	::Release(mVertexShader);
}

//////////////////////////////////////////////////////////////////////

void PhysicsDebug::Begin(Camera *camera, SpriteList *spriteList, Font *debugFont)
{
	mCamera = camera;
	mSpriteList = spriteList;
	mDebugFont = debugFont;
	mIC->Begin();
	mIC->SetMaterial(mMaterial);
	mIC->SetConstants(&camera->GetTransformMatrix(), sizeof(Matrix));
	mIC->BeginLines();
}

//////////////////////////////////////////////////////////////////////

void PhysicsDebug::End()
{
	mIC->EndLines();
	mIC->End();
}

//////////////////////////////////////////////////////////////////////

void PhysicsDebug::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	mIC->BeginVertex();
	mIC->SetPosition3(from.get128());
	mIC->SetColor(FromRGBf(color.x(), color.y(), color.z()));
	mIC->EndVertex();
	mIC->BeginVertex();
	mIC->SetPosition3(to.get128());
	mIC->SetColor(FromRGBf(color.x(), color.y(), color.z()));
	mIC->EndVertex();
}

//////////////////////////////////////////////////////////////////////

void PhysicsDebug::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
}

//////////////////////////////////////////////////////////////////////

void PhysicsDebug::reportErrorWarning(const char* warningString)
{
}

//////////////////////////////////////////////////////////////////////

void PhysicsDebug::draw3dText(const btVector3& location,const char* textString)
{
	using namespace DirectX;
	XMVECTOR v = XMVectorSet(location.x(), location.y(), location.z(), 1.0f);
	XMVECTOR sp = XMVector3TransformCoord(v, mCamera->GetTransformMatrix());
	Vec2 screenPos((XMVectorGetX(sp) + 1) / 2 * Graphics::FWidth(), (-XMVectorGetY(sp) + 1) / 2 * Graphics::FHeight());
	mDebugFont->DrawString(mSpriteList, textString, screenPos, Font::HCentre, Font::VCentre);
}

//////////////////////////////////////////////////////////////////////

void PhysicsDebug::setDebugMode(int debugMode)
{
	mDebugModes = debugMode;
}

//////////////////////////////////////////////////////////////////////

int PhysicsDebug::getDebugMode() const
{
	return mDebugModes;
}
