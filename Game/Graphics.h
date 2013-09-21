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

	void	UpdateMouse();

	extern uint	gDrawCalls;
};

extern ID3D11Device *			D3DDevice;
extern ID3D11DeviceContext *	D3DContext;