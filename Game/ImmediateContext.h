//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class ImmediateContext : public RefCount
{
public:

	static ImmediateContext *Create(VertexElement::Field vertexFieldMask, uint maxVerts, size_t constantBufferSize);

	void Begin();
	void SetMaterial(Material const *material);
	void SetBlendMode(BlendMode blendMode);
	void SetZBufferMode(ZBufferMode mode);
	void SetTexture(Texture const *texture);
	void SetViewport(int left, int top, int right, int bottom);
	void SetConstants(void const *data, size_t size);		// you better do right by the Vertex Shader constants....
	void BeginStrip();
	void BeginTriangles();
	void BeginLines();
	void BeginLineStrip();
	void BeginVertex();

	void SetPosition2(float x, float y);	// they get stuffed into the vertex buffer in the order they're set.... (for now)
	void SetPosition2(Vec2 const &pos);		// they get stuffed into the vertex buffer in the order they're set.... (for now)

	void SetPosition3(float x, float y, float z);
	void SetPosition3(Vec4f pos);

	void SetTexCoord(float u, float v);
	void SetTexCoord(Vec2 const &uv);

	void SetNormal(float x, float y, float z);
	void SetNormal(Vec4f normal);

	void SetColor(Color color);

	void EndVertex();
	void EndStrip();
	void EndTriangles();
	void EndLines();
	void EndLineStrip();
	void End();

private:

	ImmediateContext(VertexElement::Field vertexFieldMask, uint maxVerts, size_t constantBufferSize);
	~ImmediateContext();

	static const int	kNumBuffers = 2;

	VertexBuffer *		mVertexBuffer[kNumBuffers];
	ConstantBuffer *	mConstantBuffer[kNumBuffers];
	size_t				mConstantBufferSize;
	byte *				mCommandBuffer;
	byte *				mCommandBufferPointer;
	byte *				mCommandBufferEnd;
	size_t				mCommandBufferSize;
	uint32				mBufferIndex;
	uint32				mMaxVerts;
	size_t				mVertexSize;
public:
	byte *				mVertexBufferBase;
	byte *				mVertexBufferPointer;
	byte *				mVertexBufferEnd;
	uint32				mVertexIndex;
	uint32				mNumCommands;
	Material const *	mCurrentMaterial;
};

