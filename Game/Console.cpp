//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

Console::Console()
	: mActive(false)
	, mHeight(20)
{
	uint32 mask = VertexElement::Position2 | VertexElement::Color;
	mScreenProjection = Camera::OrthoProjection2D(Graphics::Width(), Graphics::Height());
	m2DSimplePixelShader = PixelShader::Load(L"2DSimplePixelShader");
	m2DSimpleVertexShader = VertexShader::Load(L"2DSimpleVertexShader", mask);
	mBlackMaterial = Material::Create(m2DSimplePixelShader, m2DSimpleVertexShader, BM_Modulate);
	mDrawContext = ImmediateContext::Create(mask, 16384, 64);
	mDrawContext->SetZBufferMode(ZB_Disable);
	mSpriteList = SpriteList::Create();
	mFont = Font::Load(L"console");
	Trace("Console");
	mTimer.Reset();
}

//////////////////////////////////////////////////////////////////////

Console::~Console()
{
	::Release(m2DSimpleVertexShader);
	::Release(m2DSimplePixelShader);
	::Release(mDrawContext);
	::Release(mBlackMaterial);
	::Release(mSpriteList);
	::Release(mFont);}

//////////////////////////////////////////////////////////////////////

Console *Console::Create()
{
	return new Console();
}

//////////////////////////////////////////////////////////////////////

void Console::Trace(char const *msg, ...)
{
	va_list v;
	va_start(v, msg);
	char buffer[8192];
	_vsnprintf_s(buffer, ARRAYSIZE(buffer) - 1, msg, v);
	mLines.push_back(buffer);
}

//////////////////////////////////////////////////////////////////////

void Console::Update()
{
	if(App::LastCharPressed == '`')
	{
		mActive = !mActive;
		mTimer.Reset();
		App::LastCharPressed = 0;
	}
	if(mActive)
	{
		if(App::LastCharPressed != 0)
		{
			mTimer.Reset();

			switch(App::LastCharPressed)
			{
			case VK_BACK:
				if(!mCommandLine.empty())
				{
					mCommandLine.pop_back();
				}
				break;

			case '#':
				mCommandLine.push_back('\\');
				mCommandLine.push_back('#');
				break;

			case '@':
				mCommandLine.push_back('\\');
				mCommandLine.push_back('@');
				break;

			case VK_RETURN:
				Trace("%s", mCommandLine.c_str());
				mCommandLine.clear();
				break;

			default:
				mCommandLine.push_back((char)App::LastCharPressed);
				break;
			}
			App::LastCharPressed = 0;
		}
		App::ResetAllInput();
	}
}

//////////////////////////////////////////////////////////////////////

void Console::Draw()
{
	if(mActive)
	{
		mTimer.Update();
		int w = Graphics::Width();
		int h = mFont->GetHeight() * mHeight;
		Vec2 p[4] =
		{
			Vec2(0, 0),
			Vec2(w, 0),
			Vec2(0, h),
			Vec2(w, h)
		};
		mDrawContext->Begin();
		mDrawContext->SetMaterial(mBlackMaterial);
		mDrawContext->SetConstants(&mScreenProjection, sizeof(Matrix));
		mDrawContext->SetZBufferMode(ZB_Disable);
		mDrawContext->BeginStrip();
		for(auto v : p)
		{
			mDrawContext->BeginVertex();
			mDrawContext->SetPosition2(v);
			mDrawContext->SetColor(0xC0000000);
			mDrawContext->EndVertex();
		}
		mDrawContext->EndStrip();
		mDrawContext->End();

		mSpriteList->Begin();
		mSpriteList->SetZBufferMode(ZB_Disable);

		float y = (float)h;
		y -= mFont->GetHeight();
		string cmd = Format("#FF00FFFF#%s#FFFFFFFF#%s", mCommandLine.c_str(), (fmod(mTimer.Elapsed(), 0.5) < 0.25) ? "_" : " ");
		float x = 0;
		Font::HorizontalAlign a = Font::HorizontalAlign::HLeft;
		Vec2 offset;
		Vec2 size = mFont->MeasureString(cmd.c_str(), &offset);
		if(size.x > Graphics::FWidth())
		{
			x = Graphics::FWidth();
			a = Font::HorizontalAlign::HRight;
		}
		mFont->BeginMultipleStrings(mSpriteList);
		Vec2 v(x, y);
		mFont->DrawStringMultiple(cmd.c_str(), v, a);
		for(auto p = mLines.rbegin(); p != mLines.rend() && y >= 0; ++p)
		{
			y -= mFont->GetHeight();
			Vec2 pos(0.0f, y);
			mFont->DrawStringMultiple(p->c_str(), pos);
		}

		mFont->EndMultipleStrings();
		mSpriteList->End();
	}
}