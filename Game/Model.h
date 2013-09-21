//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class VertexBuffer;
class IndexBuffer;
class Mesh;

//////////////////////////////////////////////////////////////////////

class Model : public RefCount
{
public:

	static Model *	Create(VertexBuffer *vertexBuffer, IndexBuffer *indexBuffer);
	void			Draw(Camera &camera);
	ModelNode *		GetRootNode();

private:

	Model();
	~Model();

	VertexBuffer *	mVertexBuffer;
	IndexBuffer *	mIndexBuffer;
	ModelNode		mRootNode;
};
