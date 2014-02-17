//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

namespace Graphics
{
	void	SetViewport(int left, int top, int right, int bottom);

	void	BeginFrame(Color clearColor);
	void	EndFrame();

	int		Width();
	int		Height();

	float	FWidth();
	float	FHeight();

	void	DrawIndexed(MeshType type, uint vertexBase, uint indexBase, uint indexCount);
	void	Draw(MeshType type, uint vertexBase, uint vertexCount);

	void	SetBlendMode(BlendMode blendMode);
	void	SetZBufferMode(ZBufferMode mode);

	bool	Open();
	bool	Update();
	void	Close();

	template<UINT TNameLength> inline void SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_z_ const char (&name)[TNameLength])
	{
#if defined(_DEBUG) || defined(PROFILE)
		resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#endif
	}

	inline void SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ string const &name)
	{
#if defined(_DEBUG) || defined(PROFILE)
		resource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length() - 1, name.c_str());
#endif
	}

	extern uint	gDrawCalls;
	extern HWND hwnd;
};

extern ID3D11Device *			D3DDevice;
extern ID3D11DeviceContext *	D3DContext;