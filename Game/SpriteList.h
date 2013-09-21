//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class SpriteList : public RefCount, Aligned16
{
public:

	static SpriteList *Create();

	void Begin();
	void SetTexture(Texture const *texture);
	void SetBlendMode(BlendMode blendMode);
	void SetZBufferMode(ZBufferMode mode);
	void SetViewport(int left, int top, int right, int bottom);
	void AddSprite(Vec2 const &topLeft, Vec2 const &size, Vec2 const &uvTopLeft, Vec2 const &uvBottomRight, Color color);
	void AddSprite(Vec2 const &topLeft, Vec2 const &size, Point2D const &uvTopLeft, Point2D const &uvBottomRight, Color color);

	void BeginSpriteRun();
	void AddSpriteEx(Vec2 const &topLeft, Vec2 const &size, Point2D const &uvTopLeft, Point2D const &uvBottomRight, Color color);
	void AddSpriteEx(Vec2 const &topLeft, Vec2 const &size, Vec2 const &uvTopLeft, Vec2 const &uvBottomRight, Color color);
	void EndSpriteRun();
	void End();

private:

	SpriteList();
	~SpriteList();

	Matrix				mProjection;
	ImmediateContext *	mIC;
	Material *			mMaterial;
	VertexShader *		mVertexShader;
	PixelShader *		mPixelShader;
	float				mTextureWidth;
	float				mTextureHeight;

};