//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Graphics.h"

//////////////////////////////////////////////////////////////////////

ID3D11Device *						D3DDevice = null;
ID3D11DeviceContext *				D3DContext = null;

uint								Graphics::gDrawCalls = 0;

//////////////////////////////////////////////////////////////////////

static HWND							hwnd = null;
static HINSTANCE					hinstance = GetModuleHandle(null);
static int							width = 1280;
static int							height = 768;
//static int							width = 640;
//static int							height = 480;
static float						fwidth = (float)width;
static float						fheight = (float)height;
static bool							active;

static TCHAR const *				windowClassName = TEXT("WINDOWCLASS");
static TCHAR const *				windowTitle = TEXT("WINDOW");
static uint							windowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;

static const DXGI_FORMAT			kBackBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

#if defined(_DEBUG)
static ID3D11Debug *				debug = null;
#endif

static IDXGISwapChain *				swapChain = null;
static ID3D11RenderTargetView *		backBuffer = null;
static ID3D11DepthStencilView *		depthStencilView = null;
static ID3D11DepthStencilState *	depthStencilState[ZB_NumZBufferModes] = { 0 };
static ID3D11Texture2D *			depthBuffer = null;
static ID3D11RasterizerState *		rasterizerState = null;

static ID3D11BlendState *			blendState[BM_NumBlendModes] = { 0 };

//////////////////////////////////////////////////////////////////////

static bool						CreateTheWindow();
static LRESULT CALLBACK			WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static bool						InitD3D();
static void						Flip();
static void						Clear(Color color);
static void						ClearDepth();

//////////////////////////////////////////////////////////////////////

bool Graphics::Open()
{
	return CreateTheWindow() && InitD3D();
}

//////////////////////////////////////////////////////////////////////

void Graphics::UpdateMouse()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);
	App::MousePosition = Vec2((float)p.x, (float)p.y);
	if(App::GetMouseMode() == App::MouseMode::Captured)
	{
		POINT c;
		c.x	= width / 2;
		c.y	= height / 2;
		App::MouseDelta = Vec2(p.x - c.x, p.y - c.y);
		ClientToScreen(hwnd, &c);
		SetCursorPos(c.x, c.y);
	}
}

//////////////////////////////////////////////////////////////////////

void Graphics::BeginFrame(Color clearColor)
{
	UpdateMouse();
	Clear(clearColor);
	ClearDepth();
	gDrawCalls = 0;
}

//////////////////////////////////////////////////////////////////////

static D3D11_PRIMITIVE_TOPOLOGY sTopologies[MeshType::MT_NumTypes] =
{
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
};

//////////////////////////////////////////////////////////////////////

void Graphics::DrawIndexed(MeshType type, uint vertexBase, uint indexBase, uint indexCount)
{
	++gDrawCalls;
	D3DContext->IASetPrimitiveTopology(sTopologies[type]);
	D3DContext->DrawIndexed(indexCount, indexBase, vertexBase);
}

//////////////////////////////////////////////////////////////////////

void Graphics::Draw(MeshType type, uint vertexBase, uint vertexCount)
{
	++gDrawCalls;
	D3DContext->IASetPrimitiveTopology(sTopologies[type]);
	D3DContext->Draw(vertexCount, vertexBase);
}

//////////////////////////////////////////////////////////////////////

void Graphics::EndFrame()
{
	Flip();
	App::MouseWheelDelta = 0;
}

//////////////////////////////////////////////////////////////////////

int Graphics::Width()
{
	return width;
}

//////////////////////////////////////////////////////////////////////

float Graphics::FWidth()
{
	return fwidth;
}

//////////////////////////////////////////////////////////////////////

int Graphics::Height()
{
	return height;
}

//////////////////////////////////////////////////////////////////////

float Graphics::FHeight()
{
	return fheight;
}

//////////////////////////////////////////////////////////////////////

void Graphics::SetViewport(int left, int top, int right, int bottom)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)(right - left);
	vp.Height = (float)(bottom - top);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = (float)left;
	vp.TopLeftY = (float)top;
	D3DContext->RSSetViewports(1, &vp);
}

//////////////////////////////////////////////////////////////////////

void Graphics::SetBlendMode(BlendMode blendMode)
{
	D3DContext->OMSetBlendState(blendState[blendMode], null, 0xffffffff);
}

//////////////////////////////////////////////////////////////////////

void Graphics::SetZBufferMode(ZBufferMode mode)
{
	D3DContext->OMSetDepthStencilState(depthStencilState[mode], 1);
}

//////////////////////////////////////////////////////////////////////

void Graphics::Close()
{
	for(uint i=0; i<BM_NumBlendModes; ++i)
	{
		Release(blendState[i]);
	}
	for(uint i=0; i<ZB_NumZBufferModes; ++i)
	{
		Release(depthStencilState[i]);
	}
	Release(rasterizerState);
	Release(depthStencilView);
	Release(depthBuffer);
	Release(backBuffer);
	Release(swapChain);

	D3DContext->ClearState();
	D3DContext->Flush();

	Release(D3DContext);
	Release(D3DDevice);

#if defined(_DEBUG)
	if(debug != null)
	{
		TRACE("D3D ==================================================================\n");
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		TRACE("======================================================================\n");
	}
	Release(debug);
#endif

	CloseWindow(hwnd);
}

//////////////////////////////////////////////////////////////////////

bool Graphics::Update()
{
	MSG msg;
	if(!active)
	{
		WaitMessage();
	}
	App::LastKeyPressed = 0;
	App::MousePressed = 0;
	App::MouseReleased = 0;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_QUIT)
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////

bool CreateTheWindow()
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WindowProc;
	wcex.hInstance		= hinstance;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName	= windowClassName;

	if(RegisterClassEx(&wcex) == 0)
	{
		return false;
	}

	RECT rc = { 0, 0, width, height };

	AdjustWindowRect(&rc, windowStyle, false);

	int wide = rc.right - rc.left;
	int high = rc.bottom - rc.top;

	hwnd = CreateWindow(windowClassName, windowTitle, windowStyle,
						GetSystemMetrics(SM_CXMAXIMIZED) / 2 - wide / 2,
						GetSystemMetrics(SM_CYMAXIMIZED) / 2 - high / 2,
						wide, high, null, null, hinstance, null);

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	POINT p;
	p.x = width/2;
	p.y = height/2;
	ClientToScreen(hwnd, &p);
	SetCursorPos(p.x, p.y);

	return hwnd != null;
}

//////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		App::LastKeyPressed = (int)wParam;
		break;

	case WM_CHAR:
		App::LastCharPressed = (int)wParam;
		break;

	case WM_ACTIVATE:
		switch(wParam)
		{
		case WA_ACTIVE:
			{
				active = true;
				if(App::GetMouseMode() == App::MouseMode::Captured)
				{
					SetCapture(hWnd);
					POINT p;
					p.x = width/2;
					p.y = height/2;
					ClientToScreen(hWnd, &p);
					SetCursorPos(p.x, p.y);
					//ShowCursor(false);
				}
			}
			break;

		case WA_INACTIVE:
			if(App::GetMouseMode() == App::MouseMode::Captured)
			{
				ReleaseCapture();
			}
			ShowCursor(true);
			active = false;
			break;

		}
		break;

	case WM_LBUTTONDOWN:
		App::MousePressed |= App::MouseButton::Left;
		App::MouseHeld |= App::MouseButton::Left;
		break;

	case WM_LBUTTONUP:
		App::MouseReleased |= App::MouseButton::Left;
		App::MouseHeld &= ~App::MouseButton::Left;
		break;

	case WM_RBUTTONDOWN:
		App::MousePressed |= App::MouseButton::Right;
		App::MouseHeld |= App::MouseButton::Right;
		break;

	case WM_RBUTTONUP:
		App::MouseReleased |= App::MouseButton::Right;
		App::MouseHeld &= ~App::MouseButton::Right;
		break;

	case WM_MOUSEWHEEL:
		{
			short delta = HIWORD(wParam);
			App::MouseWheelDelta = (float)delta/WHEEL_DELTA;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////

bool InitD3D()
{
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferCount = 1;
	scd.BufferDesc.Format = kBackBufferFormat;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;

	uint flags = 0;
	#if defined(_DEBUG)
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	DX(D3D11CreateDeviceAndSwapChain(	null, D3D_DRIVER_TYPE_HARDWARE, null, flags,
										null, 0, D3D11_SDK_VERSION, &scd, &swapChain,
										&D3DDevice, null, &D3DContext));
	Graphics::SetDebugObjectName(D3DContext, "CONTEXT");

#if defined(_DEBUG)
	DX(D3DDevice->QueryInterface(__uuidof(ID3D11Debug), (VOID **)&debug));
#endif

	ID3D11Texture2D *pBackBuffer;
	DX(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
	DX(D3DDevice->CreateRenderTargetView(pBackBuffer, null, &backBuffer));
	Graphics::SetDebugObjectName(pBackBuffer, "BACKBUFFER");
	Release(pBackBuffer);

	D3D11_TEXTURE2D_DESC descDepth = { 0 };
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DX(D3DDevice->CreateTexture2D(&descDepth, NULL, &depthBuffer));
	Graphics::SetDebugObjectName(depthBuffer, "DEPTHBUFFER");

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = descDepth.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	DX(D3DDevice->CreateDepthStencilView(depthBuffer, &dsvd, &depthStencilView));
	Graphics::SetDebugObjectName(depthStencilView, "DEPTHSTENCILVIEW");

	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	DX(D3DDevice->CreateDepthStencilState(&dsDesc, &depthStencilState[ZB_Enable]));
	Graphics::SetDebugObjectName(depthStencilState[ZB_Enable], "DSS_ENABLE");

	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	DX(D3DDevice->CreateDepthStencilState(&dsDesc, &depthStencilState[ZB_Disable]));
	Graphics::SetDebugObjectName(depthStencilState[ZB_Disable], "DSS_DISABLE");

	CD3D11_RASTERIZER_DESC rd(D3D11_DEFAULT);
//	rd.CullMode = D3D11_CULL_NONE;
//	rd.FillMode = D3D11_FILL_WIREFRAME;
	DX(D3DDevice->CreateRasterizerState(&rd, &rasterizerState));
	D3DContext->RSSetState(rasterizerState);
	Graphics::SetDebugObjectName(rasterizerState, "MYRASTERIZER");

	CD3D11_BLEND_DESC bd(D3D11_DEFAULT);
	bd.RenderTarget[0].BlendEnable = FALSE;
	DX(D3DDevice->CreateBlendState(&bd, &blendState[BM_None]));
	Graphics::SetDebugObjectName(blendState[BM_None], "BS_NONE");

	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DX(D3DDevice->CreateBlendState(&bd, &blendState[BM_Additive]));
	Graphics::SetDebugObjectName(blendState[BM_Additive], "BS_ADDITIVE");

	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DX(D3DDevice->CreateBlendState(&bd, &blendState[BM_Modulate]));
	Graphics::SetDebugObjectName(blendState[BM_Modulate], "BS_MODULATE");

	Graphics::SetViewport(0, 0, width, height);

	//D3DContext->OMSetRenderTargets(1, &backBuffer, null);
	D3DContext->OMSetRenderTargets(1, &backBuffer, depthStencilView);
	D3DContext->OMSetDepthStencilState(depthStencilState[ZB_Enable], 1);

	//Texture::Create(8, 8, 0xffffffff);

	return true;
}

//////////////////////////////////////////////////////////////////////

void Flip()
{
	DX(swapChain->Present(1, 0));
}

//////////////////////////////////////////////////////////////////////

void Clear(Color color)
{
	float abgr[4];
	for(uint i=0; i<4; ++i)
	{
		abgr[i] = ((color >> (i * 8)) & 0xff) / 255.0f;
	}
	D3DContext->ClearRenderTargetView(backBuffer, abgr);
}

//////////////////////////////////////////////////////////////////////

void ClearDepth()
{
	D3DContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
