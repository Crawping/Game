//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Axes
{
public:

	Axes();
	~Axes();

	void Begin();

	void Draw(Camera &camera, Vec4 origin, Vec4 extents, Color x, Color y, Color z);
	void DrawGrid(Camera &camera, Vec4 origin, Vec4 extents, int step, Color color);

	void BeginLines(Camera &camera);
	void DrawLine(Vec4 a, Vec4 b, Color ca, Color cb);
	void EndLines();

	void End();

private:

	ImmediateContext *		mIC;
	VertexShader *			mVS;
	PixelShader *			mPS;
	Material *				mMaterial;
};

//////////////////////////////////////////////////////////////////////

