//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Axes
{
public:

	Axes();
	~Axes();

	void Begin();

	void Draw(Camera &camera, Vector origin, Vector extents, Color x, Color y, Color z);
	void DrawGrid(Camera &camera, Vector origin, Vector extents, int step, Color color);

	void BeginLines(Camera &camera);
	void DrawLine(Vector a, Vector b, Color ca, Color cb);
	void EndLines();

	void End();

private:

	ImmediateContext *		mIC;
	VertexShader *			mVS;
	PixelShader *			mPS;
	Material *				mMaterial;
};

//////////////////////////////////////////////////////////////////////

