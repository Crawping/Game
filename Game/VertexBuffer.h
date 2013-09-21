//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class VertexBuffer : public RefCount
{
public:

	static VertexBuffer *	Create(uint numvertices, VertexElement::Field vertexFields, void *vertices = null);

	void					Activate();
	byte *					Map();
	void					UnMap();

private:

	VertexBuffer();
	~VertexBuffer();

	class VertexBufferImpl;
	VertexBufferImpl *pImpl;
};

//////////////////////////////////////////////////////////////////////

