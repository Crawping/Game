//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Axes
{
public:

	Axes();
	~Axes();

	void Begin();

	void Draw(Camera &camera, Vec3 const &origin, Vec3 const &extents, Color x, Color y, Color z);
	void DrawGrid(Camera &camera, Vec3 const &origin, Vec3 const &extents, int step, Color color);

	void BeginLines(Camera &camera);
	void DrawLine(Vec3 const &a, Vec3 const &b, Color ca, Color cb);
	void EndLines();

	void End();

private:

	ImmediateContext *		mIC;
	VertexShader *			mVS;
	PixelShader *			mPS;
	Material *				mMaterial;
};

//////////////////////////////////////////////////////////////////////

