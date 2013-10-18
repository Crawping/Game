//////////////////////////////////////////////////////////////////////
// Engine shape, body
// Draw things properly
// Proper engine behaviour
// Fix the spring joints to the car and hub directly
// Physics Objects which clean themselves up!
// Assimp -> Model
// Particles
// Multi texturing
// Reflection mapping
// Skinning
// Fix immediate context (& make SpriteList derive from it)
// Command Line parameters (log console etc)
// Tidy up source into folders/multiplatform

#include "stdafx.h"
#include "assimp\scene.h"
#include "assimp\Importer.hpp"
#include "MyApp.h"

//////////////////////////////////////////////////////////////////////

MyApp app;

//////////////////////////////////////////////////////////////////////

struct Vert
{
	Vec3	mPosition;
	Vec2		mTexCoord;
	Color		mColor;
};

#pragma push_macro("N")
#pragma push_macro("C")
#pragma push_macro("V3")
#pragma push_macro("V2")
#define N 0.5f
#define C 0xffffffff	// ABGR
#define V3 Vec3
#define V2 Vec2

Vert verts[36] =
{
	{V3(-N,-N,-N),V2(1,0),C},{V3(-N, N,-N),V2(1,1),C},{V3( N, N,-N),V2(0,1),C},
	{V3( N, N,-N),V2(0,1),C},{V3( N,-N,-N),V2(0,0),C},{V3(-N,-N,-N),V2(1,0),C},
	{V3(-N,-N, N),V2(0,0),C},{V3( N,-N, N),V2(1,0),C},{V3( N, N, N),V2(1,1),C},
	{V3( N, N, N),V2(1,1),C},{V3(-N, N, N),V2(0,1),C},{V3(-N,-N, N),V2(0,0),C},
	{V3(-N,-N,-N),V2(0,0),C},{V3( N,-N,-N),V2(1,0),C},{V3( N,-N, N),V2(1,1),C},
	{V3( N,-N, N),V2(1,1),C},{V3(-N,-N, N),V2(0,1),C},{V3(-N,-N,-N),V2(0,0),C},
	{V3( N,-N,-N),V2(0,0),C},{V3( N, N,-N),V2(1,0),C},{V3( N, N, N),V2(1,1),C},
	{V3( N, N, N),V2(1,1),C},{V3( N,-N, N),V2(0,1),C},{V3( N,-N,-N),V2(0,0),C},
	{V3( N, N,-N),V2(0,0),C},{V3(-N, N,-N),V2(1,0),C},{V3(-N, N, N),V2(1,1),C},
	{V3(-N, N, N),V2(1,1),C},{V3( N, N, N),V2(0,1),C},{V3( N, N,-N),V2(0,0),C},
	{V3(-N, N,-N),V2(0,0),C},{V3(-N,-N,-N),V2(1,0),C},{V3(-N,-N, N),V2(1,1),C},
	{V3(-N,-N, N),V2(1,1),C},{V3(-N, N, N),V2(0,1),C},{V3(-N, N,-N),V2(0,0),C},
};

#pragma  pop_macro("N")
#pragma  pop_macro("C")
#pragma  pop_macro("V3")
#pragma  pop_macro("V2")

uint16 indices[36] =
{
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35
};

//////////////////////////////////////////////////////////////////////

Camera *gCurrentCamera = null;

//Assimp::Importer assimporter;
//aiScene const *scene = null;

const int step = 64;

const int editMargin = 300;

vector<Vert>	vertex(step * 2 + (step + 1) * 2);
vector<uint16>	index;

void MyApp::OnInit()
{
	//scene = assimporter.ReadFile("data\\cube.dae", 0);
	//DumpNode(scene, scene->mRootNode, 0);

	mCylinderShape = null; 

	mCar = null;

	int gx = editMargin;
	int gy = 0;
	int gw = (Graphics::Width() - gx) / 2;
	int gh = (Graphics::Height() - gy) / 2;
	int mx = gx + gw; int my = gy + gh;
	int fx = mx + gw; int fy = my + gh;
	float aspect = (float)gw/gh;
	mViewWindow[0] = new ViewWindow(gx, gy, mx, my, Axis::X_Axis, Axis::X_Axis, Axis::X_Axis, Axis::X_Axis, false);
	mViewWindow[1] = new ViewWindow(mx, gy, fx, my, Axis::X_Axis, Axis::Z_Axis, Axis::Y_Axis, Axis::Z_Axis, true);
	mViewWindow[2] = new ViewWindow(gx, my, mx, fy, Axis::Y_Axis, Axis::Z_Axis, Axis::X_Axis, Axis::Z_Axis, true, -1.0f);
	mViewWindow[3] = new ViewWindow(mx, my, fx, fy, Axis::Z_Axis, Axis::Y_Axis, Axis::X_Axis, Axis::Y_Axis, true);

	mViewWindow[0]->mTargetZoom = 10;

	mAppParameterSets.mFilename = TEXT("config.xml");
	mAppParameterSets.AddParameterSet(&mCameraParameters);
	for(int i=0; i<4; ++i)
	{
		mViewWindow[i]->mViewParameters.mName = Format("View%d", i);
		mAppParameterSets.AddParameterSet(&mViewWindow[i]->mViewParameters);
	}
	mAppParameterSets.Load();

	for(int i=0; i<4; ++i)
	{
		mViewWindow[i]->LoadParameters();
	}

	mCameraHeight = mCameraParameters.Height;
	mCameraTargetHeight = mCameraParameters.TargetHeight;
	mCameraDistance = mCameraParameters.Distance;

	mPhysicsDebug = new PhysicsDebug();

	const float g = (float)(M_PI * 2/step);
	for(int i=0; i<step; ++i)
	{
		float r = g * i;
		float x = sinf(r);
		float y = cosf(r);
		Vert v;
		v.mPosition = Vec3(x * 0.5f, 0.5f, y * 0.5f);
		v.mTexCoord = Vec2(x * 0.5f + 0.5f, y * 0.5f + 0.5f);
		v.mColor = FromRGBA(255, 255, 255, 255);
		vertex[i] = v;

		v.mPosition.y = -0.5f;
		vertex[step + i] = v;
	}

	for(int i=0; i<=step; ++i)
	{
		float r = g * i;
		float x = sinf(r);
		float y = cosf(r);
		Vert v;
		v.mPosition = Vec3(x * 0.5f, 0.5f, y * 0.5f);
		v.mTexCoord = Vec2(x * 0.5f + 0.5f, y * 0.5f + 0.5f);
		v.mColor = FromRGBA(255, 255, 255, 255);
		v.mTexCoord.x = (float)i / step;
		v.mPosition.y = 0.5f;
		v.mTexCoord.y = 1;
		vertex[step * 2 + i] = v;

		v.mPosition.y = -0.5f;
		v.mTexCoord.y = 0;
		vertex[step * 3 + i + 1] = v;
	}

	{
		uint32 mask = VertexElement::Position3 + VertexElement::Color;
		mLinesVS = VertexShader::Load(L"UntexturedVertexShader", mask);
		mLinesPS = PixelShader::Load(L"UntexturedPixelShader");
		mLinesMaterial = Material::Create(mLinesPS, mLinesVS, BM_Modulate);
		mLinesIC = ImmediateContext::Create(mask, 1048576, 65536);
	}

	uint32 mask = VertexElement::Position3 + VertexElement::TexCoord + VertexElement::Color;

	mCylinderVB = VertexBuffer::Create(vertex.size(), mask, &vertex[0]);

	for(uint16 i=0; i<step-1; ++i)
	{
		index.push_back(0);
		index.push_back(i);
		index.push_back(i+1);
	}

	for(uint16 i=0; i<step-1; ++i)
	{
		index.push_back(step);
		index.push_back(step+i+1);
		index.push_back(step+i);
	}

	uint16 s2 = step * 2;
	uint16 s3 = step * 3 + 1;
	for(uint16 i=0; i<step; ++i)
	{
		uint16 i1 = i + 1;
		index.push_back(s2 + i);
		index.push_back(s3 + i);
		index.push_back(s3 + i1);

		index.push_back(s2 + i);
		index.push_back(s3 + i1);
		index.push_back(s2 + i1);
	}

	mCylinderIB = IndexBuffer::Create(&index[0], index.size(), IndexBuffer::IT_UINT16);

	uint32 simpleMask = VertexElement::Position3 + VertexElement::Color;

	mTexture = Texture::Load(L"Data\\redball.dds");
	mVertexShader = VertexShader::Load(L"VertexShader", mask);
	mPixelShader = PixelShader::Load(L"PixelShader");
	mMaterial = Material::Create(mPixelShader, mVertexShader, BM_None, mTexture);

	mCylinderMesh = Mesh::Create(MT_Triangles, 0, 0, index.size(), mMaterial);
	mCylinder = Model::Create(mCylinderVB, mCylinderIB);
	mCylinder->GetRootNode()->AddMesh(mCylinderMesh);

	mTransformConstantBufferIndex = mVertexShader->GetConstantBufferIndex("TransformMatrix");

	mIndexBuffer = IndexBuffer::Create(indices, ARRAYSIZE(indices), IndexBuffer::IT_UINT16);
	mVertexBuffer = VertexBuffer::Create(ARRAYSIZE(verts), mask, verts);
	mMesh = Mesh::Create(MT_Triangles, 0, 0, 36, mMaterial);

	mFixedSysFont = Font::Load(L"console");
	mDebugFont = Font::Load(L"debug");
	mBannerFont = Font::Load(L"banner");

	mImmediateContext = ImmediateContext::Create(mask, 65536, sizeof(Matrix));

	mSpriteList = SpriteList::Create();

	mask = VertexElement::Position2 | VertexElement::Color;
	m2DUntexturedVS = VertexShader::Load(L"2DSimpleVertexShader", mask);
	m2DUntexturedPS = PixelShader::Load(L"2DSimplePixelShader");
	mUntexturedMaterial = Material::Create(m2DUntexturedPS, m2DUntexturedVS, BM_Modulate);
	m2DUntexturedIC = ImmediateContext::Create(mask, 65536, 65536);

	mOldMouseDelta = Vec2(0,0);

	mCarOrientation = IdentityMatrix;

	mCameraYaw = HALF_PI;
	mCameraPitch = PI + QUARTER_PI;
	mCameraPos = Vec4(25, 0, 25);
	mTarget = Vec4(35, 0, 5);
	mYaw = mPitch = mRoll = 0;

	UpdateCamera();

	gCurrentCamera = &mCamera;

	InitPhysics();

	mEditMode = EditMode::Edit;
	mParameterIndex = 0;
	mScrollBarPos = 0;
	mScrollBarVelocity = 0;

	CountParameters();

	mConsole = Console::Create();

	mConsole->Trace("Built %s %s\n", __DATE__, __TIME__);

	mModel = Model::Create(mVertexBuffer, mIndexBuffer);
	ModelNode *node = mModel->GetRootNode();
	node->AddMesh(mMesh);

	mTimer.Reset();
	mFrame = 0;
}

//////////////////////////////////////////////////////////////////////

void MyApp::DrawViewWindow(ViewWindow *w)
{
	int l = w->mLeft;
	int r = w->mRight;
	int t = w->mTop;
	int b = w->mBottom;
	int gw = r - l;
	int gh = b - t;
	float aspect = (float)gw/gh;
	Vec2 panScale(w->mZoom / gw, w->mZoom / aspect / gh);

	btTransform const &carTransform = mCar->mBody->getWorldTransform();
	Vec4f carPosition = carTransform.getOrigin().get128();

	bool in = MousePosition.x > l && MousePosition.x < r && MousePosition.y > t && MousePosition.y < b;

	switch(w->mControlMode)
	{
	case Idle:
		if(in && MousePressed == MouseButton::Left && w->mOrtho)
		{
			w->mControlMode = Pan;
			SetMouseMode(MouseMode::Captured);
		}
		else if(in && MousePressed == MouseButton::Right && !w->mOrtho)
		{
			w->mControlMode = Rotate;
			SetMouseMode(MouseMode::Captured);
		}
		break;

	case Pan:
		if(!(MouseHeld & MouseButton::Left))
		{
			SetMouseMode(MouseMode::Free);
			w->mControlMode = Idle;
		}
		break;

	case Rotate:
		if(!(MouseHeld & MouseButton::Right))
		{
			SetMouseMode(MouseMode::Free);
			w->mControlMode = Idle;
		}
		break;
	}

	switch(w->mControlMode)
	{
	case Idle:
		if(in)
		{
			w->mTargetZoom = max((w->mTargetZoom * (1.0f - MouseWheelDelta * 0.05f)), 1.0f);
		}
		break;

	case Pan:
		w->mPan += MouseDelta * panScale;
		break;

	case Rotate:
		if(MouseDelta.x != 0 || MouseDelta.y != 0)
		{
			Vec4f v = Vec4(2.5f, 2.0f, 4.0f);
			Vec4f x = Cross(v, Vec4(0,0,1));
			v = Normalize(v);
			x = Normalize(x);
			mCarOrientation *= TranslationMatrix(carPosition * -1);
			mCarOrientation *= RotationMatrix(v, MouseDelta.x * 0.01f);
			mCarOrientation *= RotationMatrix(x, MouseDelta.y * -0.01f);
			mCarOrientation *= TranslationMatrix(carPosition);
		}
		break;
	}

	w->mZoom += (w->mTargetZoom - w->mZoom) * 0.25f;

	Graphics::SetViewport(l, t, r, b);
	if(w->mOrtho)
	{
		btTransform const &carTransform = mCar->mBody->getWorldTransform();
		Vec4f carPosition = carTransform.getOrigin().get128();
		mCamera.CalculateOrthoProjectionMatrix(w->mZoom, w->mZoom / aspect);
		int y1 = w->mYAxis;
		int x1 = w->mXAxis;
		Vec4f xaxis = carTransform.getBasis().getColumn(x1).get128() * w->mPan.x;
		Vec4f yaxis = carTransform.getBasis().getColumn(y1).get128() * w->mPan.y;
		carPosition = carPosition + xaxis + yaxis;
		Vec4f cameraPos = carPosition + carTransform.getBasis().getColumn(w->mAxis).get128() * w->mFlip * 100;
		mCamera.CalculateViewMatrix(carPosition, cameraPos, carTransform.getBasis().getColumn(w->mUpAxis).get128());
		mCamera.CalculateViewProjectionMatrix();
	}
	else
	{
		mCamera.CalculateViewMatrix(Vec4(0, 0, 5), Vec4(2.5f,2,4) * w->mZoom, Vec4(0,0,1));
		mCamera.CalculatePerspectiveProjectionMatrix(0.5f, aspect);
		mCamera.CalculateViewProjectionMatrix(mCarOrientation);
		mLinesIC->Begin();
		mLinesIC->SetMaterial(mLinesMaterial);
		mLinesIC->SetConstants(&mCamera.GetTransformMatrix(), sizeof(Matrix));
		mLinesIC->BeginLines();
		Axes::DrawGrid(mLinesIC, Vec4(0,0,0), Vec4(1000, 1000, 0), 100, 0x80ffffff);
		Axes::Draw(mLinesIC, Vec4(0,0,0), Vec4(1000, 1000, 1000), 0xff0000ff, 0xff00ff00, 0xffff0000);
		mLinesIC->EndLines();
		mLinesIC->End();
	}
	DrawPhysics();
}

//////////////////////////////////////////////////////////////////////

bool MyApp::OnUpdate()
{
	gCurrentCamera = &mCamera;
	++mFrame;

	mTimer.Update();
	mDeltaTime = (float)mTimer.Delta();
	mElapsedTime = mTimer.Elapsed();

	mConsole->Update();

	Graphics::BeginFrame(0xff204000);

	mSpriteList->Begin();
	mSpriteList->SetViewport(0, 0, Graphics::Width(), Graphics::Height());
	mSpriteList->SetZBufferMode(ZB_Disable);
	mSpriteList->SetBlendMode(BM_Modulate);

	UpdatePhysics();
	UpdateCamera();

	mRoll = mFrame / 20.0f;
	mPitch = mFrame / 30.0f;
	mYaw = mFrame / 40.0f;

	float speed = 20;
	if(KeyHeld(VK_SHIFT))
	{
		speed = 2;
	}

	switch(mEditMode)
	{
	case EditMode::Edit:
		{
			for(int i=0; i<4; ++i)
			{
				DrawViewWindow(mViewWindow[i]);
			}
			m2DUntexturedIC->Begin();
			Draw2DAxes(editMargin, 0, Graphics::Width() - editMargin, Graphics::Height());
			DrawParameters();
			m2DUntexturedIC->End();
			if(KeyPressed('D'))
			{
				mEditMode = Drive;
				Delete(mTrack);
				mTrack = new Track(32, 32);
			}
		}
		break;

	case EditMode::Drive:
		{
			if(KeyHeld('D'))
			{
				mCar->Create();
				CreateRamp();
			}
			Graphics::SetViewport(0, 0, Graphics::Width(), Graphics::Height());

			UpdateCamera();

			mLinesIC->Begin();
			mLinesIC->SetMaterial(mLinesMaterial);
			mLinesIC->SetConstants(&mCamera.GetTransformMatrix(), sizeof(Matrix));
			mLinesIC->BeginLines();
	
			Axes::DrawGrid(mLinesIC, Vec4(0,0,0), Vec4(1000, 1000, 0), 100, 0x80ffffff);

			Axes::Draw(mLinesIC, Vec4(0,0,0), Vec4(1000, 1000, 1000), 0xff0000ff, 0xff00ff00, 0xffff0000);

			mLinesIC->EndLines();
			mLinesIC->End();

			DrawPhysics();

//			DebugSetCamera(&mCamera);
//			DebugText(mCar->mBody->getWorldTransform().getOrigin().get128(), "HELLO");

			if(KeyPressed('E'))
			{
				DeleteRamp();
				Delete(mTrack);
				mEditMode = Edit;
			}
		}
		break;
	}

	mSpriteList->End();

	int framerate = (int)(0.6275f + (1.0f / mTimer.Delta()));

	DebugText(Vec2(Graphics::Width() - 120, 0), "Framerate:%d", framerate);
	DebugText(Vec2(Graphics::Width() - 120, 15), "DrawCalls:%4d", Graphics::gDrawCalls);

	DebugEndFrame();
	mConsole->Draw();

	Graphics::EndFrame();

	return LastKeyPressed != 27;
}

//////////////////////////////////////////////////////////////////////

void DrawTriangle(ImmediateContext *ic, Vec2 const &tl, Vec2 const &br, Color color)
{
	Vec2 tr(br.x, tl.y);
	Vec2 b((tl.x + br.x) / 2, br.y);
	ic->BeginTriangles();
	ic->BeginVertex();	ic->SetPosition2(tl); ic->SetColor(color); ic->EndVertex();
	ic->BeginVertex();	ic->SetPosition2(tr); ic->SetColor(color); ic->EndVertex();
	ic->BeginVertex();	ic->SetPosition2(b); ic->SetColor(color); ic->EndVertex();
	ic->EndTriangles();
}

//////////////////////////////////////////////////////////////////////

void Draw2DUntexturedRectangle(ImmediateContext *ic, Vec2 const &tl, Vec2 const &br, Color color)
{
	Vec2 tr(br.x, tl.y);
	Vec2 bl(tl.x, br.y);
	ic->BeginStrip();
	ic->BeginVertex();	ic->SetPosition2(tl); ic->SetColor(color); ic->EndVertex();
	ic->BeginVertex();	ic->SetPosition2(tr); ic->SetColor(color); ic->EndVertex();
	ic->BeginVertex();	ic->SetPosition2(bl); ic->SetColor(color); ic->EndVertex();
	ic->BeginVertex();	ic->SetPosition2(br); ic->SetColor(color); ic->EndVertex();
	ic->EndStrip();
}

//////////////////////////////////////////////////////////////////////

void MyApp::CountParameters()
{
	mNumParameterLines = 0;
	for(auto s: ParameterSet::mSets)
	{
		if(!s->mPrivate)
		{
			++mNumParameterLines;
			if(s->mExpanded)
			{
				mNumParameterLines += s->mParameters.size();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

void MyApp::DrawParameters()
{
	enum Mode
	{
		None = 0,
		Pick = 1,
		Modify = 2
	};
	static uint mode = Pick;
	static uint currentParameterIndex = 0;

	CountParameters();

	if(MouseReleased & MouseButton::Left)
	{
		if(mode == Modify)
		{
			mCar->mParameterSets.Save();
		}
		SetMouseMode(MouseMode::Free);
		currentParameterIndex = 0;
		mode = Pick;
	}
	vector<Point2D> boxes;
	float margin = editMargin;
	float sbw = 10;
	int screenPosY = 0;
	int fh = mFixedSysFont->GetHeight();
	float height = (float)(max(Graphics::Height(), mNumParameterLines * fh));
	float ratio = Graphics::FHeight() / height;
	float scrollBarHeight = ratio * Graphics::FHeight();
	float maxS = Graphics::FHeight() - scrollBarHeight;

	if(MousePosition.x > 0 && MousePosition.x < margin)
	{
		mScrollBarVelocity -= MouseWheelDelta * fh * 0.5f;
	}

	if(fabsf(mScrollBarVelocity) < 0.1f || mode == Modify)
	{
		mScrollBarVelocity = 0;
	}

	mScrollBarPos += mScrollBarVelocity;

	if(mScrollBarPos < 0)
	{
		mScrollBarPos = 0;
	}
	else if(mScrollBarPos > maxS)
	{
		mScrollBarPos = maxS;
	}

	float y = -mScrollBarPos / ratio;
	y = floorf(y + 0.5f);

	mScrollBarVelocity *= 0.8f;
	mSpriteList->SetViewport(0, 0, Graphics::Width(), Graphics::Height());
	mSpriteList->SetBlendMode(BM_Modulate);
	mSpriteList->SetZBufferMode(ZB_Disable);
	mFixedSysFont->BeginMultipleStrings(mSpriteList);
	for(auto s: ParameterSet::mSets)
	{
		if(!s->mPrivate)
		{
			Vec2 tl(0.0f, y);
			bool hover = MousePosition.x < margin - sbw && MousePosition.y >= tl.y && MousePosition.y < tl.y + fh;

			if(tl.y > -fh && tl.y < Graphics::FHeight())
			{
				Vec2 tc(tl);
				mFixedSysFont->DrawStringMultiple(Format("%s%s", hover ? "#FFFFFFFF#" : "#FF00FFFF#", s->mName.c_str()).c_str(), tc);
			}

			if(hover)
			{
				Draw2DUntexturedRectangle(m2DUntexturedIC, tl, tl + Vec2(margin - sbw, (float)fh), 0xff808080);
				if(mode == Pick && (MousePressed & MouseButton::Left))
				{
					s->mExpanded = !s->mExpanded;
				}
			}
			y += fh;

			if(s->mExpanded)
			{
				for(auto p: s->mParameters)
				{
					Vec2 tl(0.0f, y);
					bool hover = MousePosition.x < margin - sbw && MousePosition.y >= tl.y && MousePosition.y < tl.y + fh;
					if(tl.y > -fh && tl.y < Graphics::FHeight())
					{
						bool drawRect = false;
						Color textColor(0);
						Color rectColor(0);

						if(hover && mode == Pick && (MousePressed & MouseButton::Left))
						{
							// yes, set it as the selected one and start tracking the mouse delta
							SetMouseMode(MouseMode::Captured);
							currentParameterIndex = p->mIndex;
							mode = Modify;
						}

						if(mode == Modify && currentParameterIndex == p->mIndex)
						{
							textColor = 0xffffffff;
							rectColor = 0xff000000;
							drawRect = true;
						}
						else if(mode == Pick && hover)
						{
							textColor = 0xffffffff;
							rectColor = 0xff808080;
							drawRect = true;
						}
						else
						{
							textColor = 0xffd0d0d0;
							drawRect = false;
						}

						if(mode == Modify && p->mIndex == currentParameterIndex)
						{
							const int ts = 9;
							const int tw = 13;
							const int w = 500;
							Draw2DUntexturedRectangle(m2DUntexturedIC, tl + Vec2(margin, 0.0f), tl + Vec2((int)margin + w + tw, fh), 0xff000000);
							float low = p->mMinValue;
							float high = p->mMaxValue;
							float range = high - low;
							float tick = range / w;
							p->mValue += MouseDelta.x * tick;
							if(p->mValue < low)
							{
								p->mValue = low;
							}
							if(p->mValue > high)
							{
								p->mValue = high;
							}
							float xpos = (p->mValue - low) / range * w;
							DrawTriangle(m2DUntexturedIC, tl + Vec2(margin + xpos, 0.0f), tl + Vec2(margin + xpos + tw, (float)fh), 0xffffffff);
							mCar->ApplyParameters();
						}
						if(drawRect)
						{
							Draw2DUntexturedRectangle(m2DUntexturedIC, tl, tl + Vec2(margin - sbw, (float)fh), rectColor);
						}
						string s = Format("#%08x#%-24.24s %10.3f", textColor, p->mName.c_str(), p->mValue);
						mFixedSysFont->DrawStringMultiple( s.c_str(), tl);
					}
					y += fh;
				}
			}
		}
	}
	mFixedSysFont->EndMultipleStrings();

	// scroll indicator
	Draw2DUntexturedRectangle(m2DUntexturedIC, Vec2(margin-sbw, mScrollBarPos), Vec2(margin-1, mScrollBarPos + scrollBarHeight), 0xffc0c0c0);
}

//////////////////////////////////////////////////////////////////////

void MyApp::Draw2DAxes(int x, int y, int w, int h)
{
	Matrix m = Camera::OrthoProjection2D(Graphics::Width(), Graphics::Height());
	m2DUntexturedIC->SetViewport(0, 0, Graphics::Width(), Graphics::Height());
	m2DUntexturedIC->SetMaterial(mUntexturedMaterial);
	m2DUntexturedIC->SetConstants(&m, sizeof(Matrix));
	m2DUntexturedIC->SetZBufferMode(ZB_Disable);
	m2DUntexturedIC->BeginLines();

	Color border(0xffffffff);
	Color cross(0x80ffffff);

	int off = h / 2;

	for(int a = 0; a < w; a += w/2)
	{
		m2DUntexturedIC->BeginVertex();
		m2DUntexturedIC->SetPosition2(Vec2(a + x + w / 4, y + off));
		m2DUntexturedIC->SetColor(cross);
		m2DUntexturedIC->EndVertex();
		m2DUntexturedIC->BeginVertex();
		m2DUntexturedIC->SetPosition2(Vec2(a + x + w / 4, y + h));
		m2DUntexturedIC->SetColor(cross);
		m2DUntexturedIC->EndVertex();

		off = 0;

		m2DUntexturedIC->BeginVertex();
		m2DUntexturedIC->SetPosition2(Vec2(a + x, y));
		m2DUntexturedIC->SetColor(border);
		m2DUntexturedIC->EndVertex();

		m2DUntexturedIC->BeginVertex();
		m2DUntexturedIC->SetPosition2(Vec2(a + x, y + h));
		m2DUntexturedIC->SetColor(border);
		m2DUntexturedIC->EndVertex();

	}

	off = w / 2;

	for(int a = y; a < h; a += h/2)
	{
		m2DUntexturedIC->BeginVertex();
		m2DUntexturedIC->SetPosition2(Vec2(x + off, a + h / 4));
		m2DUntexturedIC->SetColor(cross);
		m2DUntexturedIC->EndVertex();
		m2DUntexturedIC->BeginVertex();
		m2DUntexturedIC->SetPosition2(Vec2(x + w, a + h / 4));
		m2DUntexturedIC->SetColor(cross);
		m2DUntexturedIC->EndVertex();

		off = 0;

		m2DUntexturedIC->BeginVertex();
		m2DUntexturedIC->SetPosition2(Vec2(x, a));
		m2DUntexturedIC->SetColor(border);
		m2DUntexturedIC->EndVertex();
		m2DUntexturedIC->BeginVertex();
		m2DUntexturedIC->SetPosition2(Vec2(x + w, a));
		m2DUntexturedIC->SetColor(border);
		m2DUntexturedIC->EndVertex();
	}
	m2DUntexturedIC->EndLines();
}

//////////////////////////////////////////////////////////////////////

void MyApp::Draw()
{
	mVertexBuffer->Activate();
	Matrix m = mCamera.GetTransformMatrix(RotationMatrix(mYaw, mPitch, mRoll));
	mVertexShader->UpdateConstants(mTransformConstantBufferIndex, &m);
	mIndexBuffer->Activate();
	Graphics::SetZBufferMode(ZB_Enable);
	mMesh->Draw();
	mModel->Draw(mCamera);

	Graphics::SetBlendMode(BM_None);
	mCylinder->Draw(mCamera);

	mMaterial->UpdateConstants(mTransformConstantBufferIndex, &mCamera.GetTransformMatrix());

//	DrawNode(scene, scene->mRootNode);

	//Vec2 results[32 + 1];
	//Vec2 controlPoints[4];
	//controlPoints[0] = Vec2(0,0);
	//controlPoints[1] = Vec2(500,0);
	//controlPoints[2] = Vec2(0,500);
	//controlPoints[3] = Vec2(500,500);
	//uint n = CalculateBezier(controlPoints, ARRAYSIZE(controlPoints), results, ARRAYSIZE(results)-1);

	//for(uint i=0; i<ARRAYSIZE(controlPoints); ++i)
	//{
	//	mAxes->Draw(mCamera, Vec3(controlPoints[i].x, controlPoints[i].y, 0), Vec3(1, 1, 1), 0xffff00ff, 0xffff00ff, 0xffff00ff);
	//}
	//mAxes->BeginLines(mCamera);
	//mDebugFont->DrawString(mSpriteList, Format("Points: %d", n).c_str(), Vec2(0,20));
	//for(uint i=0; i<n-1; ++i)
	//{
	//	mAxes->DrawLine(Vec3(results[i].x, results[i].y, 0), Vec3(results[i+1].x, results[i+1].y, 0), 0xffffffff, 0xffffffff);
	//}
	//mAxes->EndLines();
}

void MyApp::OnClose()
{
	Delete(mTrack);

	Delete(mCylinderShape);

	mCameraParameters.Height.set(mCameraHeight);
	mCameraParameters.TargetHeight.set(mCameraTargetHeight);
	mCameraParameters.Distance.set(mCameraDistance);

	for(int i=0; i<4; ++i)
	{
		mViewWindow[i]->SaveParameters();
	}

	mCar->mParameterSets.Save();
	mAppParameterSets.Save();

	DebugClose();

	CleanUpPhysics();

	Release(mLinesVS);
	Release(mLinesPS);
	Release(mLinesMaterial);
	Release(mLinesIC);

	Release(m2DUntexturedVS);
	Release(m2DUntexturedPS);
	Release(mUntexturedMaterial);
	Release(m2DUntexturedIC);

	Delete(mPhysicsDebug);
	Release(mCylinderIB);
	Release(mCylinderVB);
	Release(mCylinderMesh);
	Release(mCylinder);
	Release(mMesh);
	Release(mModel);
	Release(mConsole);
	Release(mFixedSysFont);
	Release(mDebugFont);
	Release(mBannerFont);
	Release(mSpriteList);
	Release(mImmediateContext);
	Release(mTexture);
	Release(mMaterial);
	Release(mIndexBuffer);
	Release(mVertexBuffer);
	Release(mPixelShader);
	Release(mVertexShader);
}

// masses must sum to 1

//////////////////////////////////////////////////////////////////////

void MyApp::CreateRamp()
{
	DeleteRamp();

	mTestShape[0] = new btBoxShape(btVector3(2,2,2));
	mTestShape[1] = new btBoxShape(btVector3(8,1,1));
	mTestCompoundShape = new btCompoundShape();
	btTransform a;
	btTransform b;
	a.setIdentity();
	b.setIdentity();
	a.setOrigin(btVector3(0,0,0));
	b.setOrigin(btVector3(10,0,0));
	mTestCompoundShape->addChildShape(a, mTestShape[0]);
	mTestCompoundShape->addChildShape(b, mTestShape[1]);

	float testMass(1.0f);
	btVector3 localInertia(0,0,0);

	btTransform shift;
	btTransform startTransform;
	shift.setIdentity();
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(-290, 0, 45));

	float masses[2] = { 0.99f, 0.01f };

	btCompoundShape *newShape = Physics::InitCompoundShape(mTestCompoundShape, masses, shift);
	Delete(mTestCompoundShape);
	mTestCompoundShape = newShape;

	mTestCompoundShape->calculateLocalInertia(testMass, localInertia);

	mTestBody = new btRigidBody(testMass, new btDefaultMotionState(startTransform * shift), mTestCompoundShape, localInertia);

	Physics::DynamicsWorld->addRigidBody(mTestBody, -1, -1);
	mTestBody->setActivationState(DISABLE_DEACTIVATION);
	mTestBody->setFriction(0.01f);
	mTestBody->setRestitution(0.95f);

	mBoxMesh = new btTriangleMesh();
	mBoxMesh->addTriangle(Vec4(-50, 50, -1), Vec4(-50, -50, -1), Vec4(-190, 1, 10));
	mBoxShape = new btBvhTriangleMeshShape(mBoxMesh, true);
	mBoxShape->setMargin(0.1f);
	mBoxBody = new btRigidBody(0, new btDefaultMotionState(), mBoxShape);
	Physics::DynamicsWorld->addRigidBody(mBoxBody, -1, -1);
	mBoxBody->setRestitution(0.5f);
	mBoxBody->setFriction(0);
	mBoxBody->setContactProcessingThreshold(0);

	mBallShape = new btSphereShape(6);

	for(uint i=0; i<kNumBalls; ++i)
	{
		float t = i / (float)kNumBalls * PI * 2;
		btTransform position;
		position = btTransform::getIdentity();
		position.setOrigin(Vec4(sinf(t) * 290, cosf(t) * 290, 50));
		mBalls[i] = new btRigidBody(5, new btDefaultMotionState(position), mBallShape);
		mBalls[i]->setRestitution(0.9f);
		mBalls[i]->setFriction(0.01f);
		Physics::DynamicsWorld->addRigidBody(mBalls[i], -1, -1);
	}

	mCylinderShape = new btCylinderShape(Vec4(1,1,1));
}

//////////////////////////////////////////////////////////////////////

void MyApp::DeleteRamp()
{
	Delete(mCylinderShape);

	Physics::DeleteRigidBody(mRamp);
	Physics::DeleteRigidBody(mTestBody);
	Delete(mRampShape);
	Delete(mTestShape[0]);
	Delete(mTestShape[1]);
	Delete(mTestCompoundShape);

	Delete(mBoxMesh);
	Delete(mBoxShape);
	Physics::DeleteRigidBody(mBoxBody);

	Delete(mBallShape);
	for(uint i=0; i<kNumBalls; ++i)
	{
		Physics::DeleteRigidBody(mBalls[i]);
	}
}

//////////////////////////////////////////////////////////////////////
// Steering
// Anti-roll bars

void MyApp::InitPhysics()
{
	Physics::Open();

	mGroundShape = new btStaticPlaneShape(btVector3(0,0,1),0);

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,mGroundShape,btVector3(0,0,0));
	mGroundRigidBody = new btRigidBody(groundRigidBodyCI);
	Physics::DynamicsWorld->addRigidBody(mGroundRigidBody, 1, -1);
	mGroundRigidBody->setRestitution(1);
	mGroundRigidBody->setFriction(1);

	mCar = new Car();
	mCar->Create();
}

//////////////////////////////////////////////////////////////////////

ALIGN(16, struct) ClosestConvexResultCallback : public btCollisionWorld::ConvexResultCallback
{
	ClosestConvexResultCallback(const btVector3&	convexFromWorld,const btVector3&	convexToWorld)
		:m_convexFromWorld(convexFromWorld),
		m_convexToWorld(convexToWorld),
		m_hitCollisionObject(0)
	{
	}

	btVector3	m_convexFromWorld;//used to calculate hitPointWorld from hitFraction
	btVector3	m_convexToWorld;
	btVector3	m_hitNormalWorld;
	btVector3	m_hitPointWorld;
	const btCollisionObject*	m_hitCollisionObject;

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,bool normalInWorldSpace)
	{
		//caller already does the filter on the m_closestHitFraction
		btAssert(convexResult.m_hitFraction <= m_closestHitFraction);

		m_closestHitFraction = convexResult.m_hitFraction;
		m_hitCollisionObject = convexResult.m_hitCollisionObject;
		if (normalInWorldSpace)
		{
			m_hitNormalWorld = convexResult.m_hitNormalLocal;
		} else
		{
			///need to transform normal into worldspace
			m_hitNormalWorld = m_hitCollisionObject->getWorldTransform().getBasis()*convexResult.m_hitNormalLocal;
		}
		m_hitPointWorld = convexResult.m_hitPointLocal;
		return convexResult.m_hitFraction;
	}
};

//////////////////////////////////////////////////////////////////////

void MyApp::UpdatePhysics()
{
	if(!KeyHeld('X') || KeyPressed('Z'))
	{
		float dt = 1.0f/10;
		int step = 20;
		Physics::DynamicsWorld->stepSimulation(dt, step, dt/step);
	}

	if(mEditMode == Edit)
	{
		mCar->Create();
	}
	else if(mCar->IsValid())
	{
		if(mCylinderShape != null)
		{
			mStartPos.setIdentity();
			mEndPos.setIdentity();

			mStartPos.setOrigin(Vec4(0,0,10));
			mEndPos.setOrigin(Vec4(0,0,0));

			ClosestConvexResultCallback collisionCallback(mStartPos.getOrigin(), mEndPos.getOrigin());

			Physics::DynamicsWorld->convexSweepTest(mCylinderShape, mStartPos, mEndPos, collisionCallback);

			if(collisionCallback.m_hitCollisionObject != null)
			{
				DebugSetCamera(&mCamera);
				DebugText(collisionCallback.m_hitPointWorld.get128(), "@");
			}
		}

		mCar->AntiRoll();

		float slipLeft = mCar->mWheelAssembly[Car::BackLeft]->LateralSlip();	// 1 is full slide (90 degrees), 0 is full grip
		float slipRight = mCar->mWheelAssembly[Car::BackRight]->LateralSlip();	// 1 is full slide (90 degrees), 0 is full grip
		float slip = (slipLeft + slipRight);
		float slipReducer = 1 - (slip * 0.5f);
		float power = mCar->mEngineParams.Power * slipReducer;

		if(Joypad::Trigger(0, 0) > 0)
		{
			mCar->ApplyPower(-100, power * Joypad::Trigger(0, 0));
		}
		else
		{
			mCar->ApplyPower(20000, power * Joypad::Trigger(0, 1));
		}

		if(KeyHeld(VK_UP))
		{
			mCar->ApplyPower(20000, power);
		}

		if(KeyHeld(VK_DOWN))
		{
			mCar->ApplyPower(-20, power);
		}

		float steeringf = 0.3f;

		if(KeyHeld(VK_LEFT))
		{
			mCar->mWheelAssembly[Car::FrontRight]->mSteeringHingeUpper->setLimit(steeringf, steeringf);
			mCar->mWheelAssembly[Car::FrontLeft]->mSteeringHingeUpper->setLimit(steeringf, steeringf);
			mCar->mWheelAssembly[Car::FrontRight]->mSteeringHingeLower->setLimit(steeringf, steeringf);
			mCar->mWheelAssembly[Car::FrontLeft]->mSteeringHingeLower->setLimit(steeringf, steeringf);
		}
		else if(KeyHeld(VK_RIGHT))
		{
			mCar->mWheelAssembly[Car::FrontRight]->mSteeringHingeUpper->setLimit(-steeringf, -steeringf);
			mCar->mWheelAssembly[Car::FrontLeft]->mSteeringHingeUpper->setLimit(-steeringf, -steeringf);
			mCar->mWheelAssembly[Car::FrontRight]->mSteeringHingeLower->setLimit(-steeringf, -steeringf);
			mCar->mWheelAssembly[Car::FrontLeft]->mSteeringHingeLower->setLimit(-steeringf, -steeringf);
		}
		else
		{
			float steer = Joypad::Stick(0, 0).x * -0.3f;
			mCar->mWheelAssembly[Car::FrontRight]->mSteeringHingeUpper->setLimit(steer, steer);
			mCar->mWheelAssembly[Car::FrontLeft]->mSteeringHingeUpper->setLimit(steer, steer);
			mCar->mWheelAssembly[Car::FrontRight]->mSteeringHingeLower->setLimit(steer, steer);
			mCar->mWheelAssembly[Car::FrontLeft]->mSteeringHingeLower->setLimit(steer, steer);
		}
	}
}

//////////////////////////////////////////////////////////////////////

void MyApp::UpdateCamera()
{
	static bool follow = true;

	bool oldFollow = follow;

	//DebugText("%01x\n", App::MouseHeld);

	if(KeyPressed(VK_BACK))
	{
		follow = !follow;
	}

	if(follow)
	{
		btVector3 const &cp = (mCar != null) ? mCar->mBody->getWorldTransform().getOrigin() : btVector3(0,0,0);

		// smoosh camera towards a point behind the car
		btVector3 const &d = (mCar != null) ? mCar->mBody->getWorldTransform().getBasis().getRow(0) : btVector3(-1,0,0);
		float angle = atan2f(d.x(), d.y()) - (float)M_PI / 2.0f;

		if(KeyHeld(VK_PRIOR))
		{
			mCameraDistance += 40 * mDeltaTime;
		}
		if(KeyHeld(VK_NEXT))
		{
			mCameraDistance -= 40 * mDeltaTime;
		}

		if(KeyHeld(VK_HOME))
		{
			mCameraHeight += 40 * mDeltaTime;
		}
		if(KeyHeld(VK_END))
		{
			mCameraHeight -= 40 * mDeltaTime;
		}
		if(KeyHeld(VK_INSERT))
		{
			mCameraTargetHeight += 40 * mDeltaTime;
		}
		if(KeyHeld(VK_DELETE))
		{
			mCameraTargetHeight -= 40 * mDeltaTime;
		}
		float len = mCameraDistance;

		Vec4f bcp = Vec4(cp.x() + cosf(angle) * len, cp.y() + sinf(angle) * len, 0);

		Vec4f diff2 = bcp - mCameraPos;
		mCameraPos = mCameraPos + diff2 * 0.1f;

		Vec4f car = cp.get128();
		Vec4f diff = car - mCameraPos;
		float distance = Length(diff);
		diff = Normalize(diff);
		diff = diff * (distance - len);
		mCameraPos = mCameraPos + diff * 0.5f;

		if(!oldFollow)
		{
			mCameraPos = bcp + Vec4(0,0,mCameraHeight);
		}
		Vec4f target = car + Vec4(0,0,mCameraTargetHeight);
		Vec4f pos = mCameraPos + Vec4(0,0,mCameraHeight);

		float pitch = atan2f(mCameraTargetHeight - mCameraHeight, distance);

		mCamera.CalculateViewMatrix(target, pos, Vec4(0,0,1));
		mCamera.CalculatePerspectiveProjectionMatrix(0.5f, Graphics::FWidth() / Graphics::FHeight());
		mCamera.CalculateViewProjectionMatrix();

	}
	else if((App::MouseHeld & App::MouseButton::Left) != 0)
	{
		SetMouseMode(MouseMode::Captured);
		//DebugText(Vec2(100, 100), "%f,%f", App::MouseDelta.x, App::MouseDelta.y);

		Vec2 mouseDeltaTotal = (mOldMouseDelta + App::MouseDelta) / 2;
		mOldMouseDelta = App::MouseDelta;

		mCameraYaw -= mouseDeltaTotal.x * 0.0025f;
		mCameraPitch -= mouseDeltaTotal.y * 0.0025f;
		mCamera.CalculateViewMatrix(mCameraPos, 0, mCameraPitch, mCameraYaw);

		Matrix m = XMMatrixTranspose(mCamera.GetViewMatrix());
		Vec4f dx(m.r[0]);
//		Vec4 dy(m.r[1]);
		Vec4f dz(m.r[2]);

		float speed = 0.05f;
		if(!KeyHeld(VK_SHIFT))
		{
			speed = 1.0f;
		}

		if(KeyHeld('W'))
		{
			mCameraPos = mCameraPos + dz * speed;
		}
		if(KeyHeld('S'))
		{
			mCameraPos = mCameraPos - dz * speed;
		}
		if(KeyHeld('A'))
		{
			mCameraPos = mCameraPos - dx * speed;
		}
		if(KeyHeld('D'))
		{
			mCameraPos = mCameraPos + dx * speed;
		}
		if(KeyHeld('R'))
		{
			mCameraPos = mCameraPos + Vec4(0,0,speed);
		}
		if(KeyHeld('F'))
		{
			mCameraPos = mCameraPos - Vec4(0,0,speed);
		}
		//DebugText(Vec2(50, 50), "Pos: %f,%f,%f\nYaw: %f\nPitch: %f", mCameraPos.x, mCameraPos.y, mCameraPos.z, mCameraYaw, mCameraPitch);
	}
	else
	{
		SetMouseMode(MouseMode::Free);
	}
}

//////////////////////////////////////////////////////////////////////

Matrix MatrixFromBulletTransform(btTransform const &trans)
{
	btVector3 R = trans.getBasis().getColumn(0);
	btVector3 U = trans.getBasis().getColumn(1);
	btVector3 L = trans.getBasis().getColumn(2);
	btVector3 P = trans.getOrigin();

	return Matrix(	R.x(), R.y(), R.z(), 0,
		U.x(), U.y(), U.z(), 0,
		L.x(), L.y(), L.z(), 0,
		P.x(), P.y(), P.z(), 1);
}

//////////////////////////////////////////////////////////////////////

Matrix GetTransform(btRigidBody *body)
{
	return MatrixFromBulletTransform(body->getCenterOfMassTransform());
}

//////////////////////////////////////////////////////////////////////

void MyApp::DrawPhysics()
{
	//static uint options = btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits | btIDebugDraw::DBG_DrawText | btIDebugDraw::DBG_DrawFeaturesText;
	//static uint options = btIDebugDraw::DBG_DrawWireframe| btIDebugDraw::DBG_DrawConstraints;
	static uint options = btIDebugDraw::DBG_DrawWireframe| btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawContactPoints;
	if(KeyPressed('1'))
	{
		options ^= btIDebugDraw::DBG_DrawWireframe;
	}

	Physics::DynamicsWorld->setDebugDrawer(mPhysicsDebug);
	mPhysicsDebug->setDebugMode(options);
	mPhysicsDebug->Begin(&mCamera, mSpriteList, mDebugFont);
	Physics::DynamicsWorld->debugDrawWorld();
	mPhysicsDebug->End();
}

//////////////////////////////////////////////////////////////////////

void MyApp::CleanUpPhysics()
{
	DeleteRamp();
	Delete(mCar);

	Physics::DeleteRigidBody(mGroundRigidBody);
	Delete(mGroundShape);

	Physics::Close();
}

//////////////////////////////////////////////////////////////////////

void MyApp::DumpNode(aiScene const *scene, aiNode const *node, int indent /* = 0 */)
{
	TRACE("%-*sNode %s\n", indent, "", node->mName.C_Str());
	for(uint i=0; i<node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		TRACE("%-*sMesh %s\n", indent, "", mesh->mName.C_Str());
		if(mesh->HasPositions())
		{
			TRACE("%-*s%d faces\n", indent, "", mesh->mNumFaces);
			for (uint f=0; f<mesh->mNumFaces; ++f)
			{
				aiFace *face = &mesh->mFaces[f];

				if(face->mNumIndices == 3)
				{
					for(i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
					{
						int x = face->mIndices[i];	// get group index for current index
						TRACE("%-*sPOS: %f,%f,%f\n", indent, "", mesh->mVertices[x].x, mesh->mVertices[x].y, mesh->mVertices[x].z);
						if(mesh->HasTextureCoords(0))
						{
							aiVector3D *v = &mesh->mTextureCoords[0][x];

							TRACE("%-*sUV: %f,%f\n", indent, "", v->x, v->y);
						}
						if(mesh->HasVertexColors(0))
						{
							aiColor4D *v = &mesh->mColors[0][x];
							TRACE("%-*sCOL: %f,%f,%f,%f\n", indent, "", v->r, v->g, v->b, v->a);
						}
					}
				}
			}
		}
	}
	for(uint i=0; i<node->mNumChildren; ++i)
	{
		DumpNode(scene, node->mChildren[i], indent + 2);
	}
}

//////////////////////////////////////////////////////////////////////

void MyApp::DrawNode(aiScene const *scene, aiNode const *node)
{
	for(uint i=0; i<node->mNumMeshes; ++i)
	{
		mImmediateContext->BeginTriangles();
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		if(mesh->HasPositions())
		{
			for (uint f=0; f<mesh->mNumFaces; ++f)
			{
				aiFace *face = &mesh->mFaces[f];

				if(face->mNumIndices == 3)
				{
					for(i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
					{
						int x = face->mIndices[i];	// get group index for current index
						mImmediateContext->BeginVertex();
						mImmediateContext->SetPosition3(Vec4(mesh->mVertices[x].x, mesh->mVertices[x].y, mesh->mVertices[x].z));
						if(mesh->HasTextureCoords(0))
						{
							aiVector3D *v = &mesh->mTextureCoords[0][x];
							mImmediateContext->SetTexCoord(Vec2(v->x, v->y));
						}
						else
						{
							mImmediateContext->SetTexCoord(Vec2(0.5f, 0.5f));
						}
						if(mesh->HasVertexColors(0))
						{
							aiColor4D *v = &mesh->mColors[0][x];
							mImmediateContext->SetColor(GetColorFromRGBAFloats(reinterpret_cast<float *>(&v)));
						}
						else
						{
							mImmediateContext->SetColor(0xffffffff);
						}
						mImmediateContext->EndVertex();
					}
				}
			}
		}
		mImmediateContext->EndTriangles();
	}
	for(uint i=0; i<node->mNumChildren; ++i)
	{
		DrawNode(scene, node->mChildren[i]);
	}
}

//////////////////////////////////////////////////////////////////////

