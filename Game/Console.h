//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Console : public RefCount, Aligned16
{
public:

	static Console *Create();	// dimensions etc

	void Update();
	void Trace(char const *msg, ...);
	void Draw();

private:

	Console();
	~Console();

	Matrix				mScreenProjection;
	bool				mActive;
	int					mHeight;
	SpriteList *		mSpriteList;
	Font *				mFont;
	vector<string>		mLines;
	string				mCommandLine;
	Timer				mTimer;

	// all this for drawing a black rectangle, there must be a better way...
	ImmediateContext *	mDrawContext;
	Material *			mBlackMaterial;
	PixelShader *		m2DSimplePixelShader;
	VertexShader *		m2DSimpleVertexShader;
};
