//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

class VertexBuffer::VertexBufferImpl
{
public:

	//////////////////////////////////////////////////////////////////////

	VertexBufferImpl(uint numvertices, VertexElement::Field vertexFields, void *vertices)
	{
		mStride = VertexElement::GetVertexSize(vertexFields);
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.Usage = (vertices != null) ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = mStride * numvertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = (vertices == null) ? D3D11_CPU_ACCESS_WRITE : 0;
		vertexBufferDesc.StructureByteStride = mStride;
		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;
		DX(D3DDevice->CreateBuffer(&vertexBufferDesc, (vertices != null) ? &data : null, &mVertexBuffer));
	}

	//////////////////////////////////////////////////////////////////////

	~VertexBufferImpl()
	{
		::Release(mVertexBuffer);
	}

	//////////////////////////////////////////////////////////////////////

	byte *Map()
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		DX(D3DContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		return reinterpret_cast<byte *>(msr.pData);
	}

	//////////////////////////////////////////////////////////////////////

	void UnMap()
	{
		D3DContext->Unmap(mVertexBuffer, 0);
	}

	//////////////////////////////////////////////////////////////////////

	void Activate()
	{
		uint32 offset = 0;
		D3DContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &mStride, &offset);
	}

	//////////////////////////////////////////////////////////////////////

private:

	ID3D11Buffer *		mVertexBuffer;
	uint32				mStride;
};

//////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer()
	: pImpl(null)
{
}

//////////////////////////////////////////////////////////////////////

VertexBuffer::~VertexBuffer()
{
	Delete(pImpl);
}

//////////////////////////////////////////////////////////////////////

VertexBuffer *VertexBuffer::Create(uint numvertices, VertexElement::Field vertexDescriptor, void *vertices)
{
	VertexBuffer *v = new VertexBuffer();
	v->pImpl = new VertexBufferImpl(numvertices, vertexDescriptor, vertices);
	return v;
}

//////////////////////////////////////////////////////////////////////

byte *VertexBuffer::Map()
{
	assert(pImpl != null);
	return pImpl->Map();
}

//////////////////////////////////////////////////////////////////////

void VertexBuffer::UnMap()
{
	assert(pImpl != null);
	pImpl->UnMap();
}

//////////////////////////////////////////////////////////////////////

void VertexBuffer::Activate()
{
	assert(pImpl != null);
	pImpl->Activate();
}

//////////////////////////////////////////////////////////////////////

