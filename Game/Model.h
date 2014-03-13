//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class VertexBuffer;
class IndexBuffer;
class Mesh;

//////////////////////////////////////////////////////////////////////

class Model : public Aligned16, public RefCount 
{
public:

	static Model *	Create(VertexBuffer *vertexBuffer, IndexBuffer *indexBuffer);
	void			Draw(Camera &camera);
	ModelNode *		GetRootNode();

private:

	Model();
	~Model();

	ModelNode		mRootNode;
	VertexBuffer *	mVertexBuffer;
	IndexBuffer *	mIndexBuffer;
};
