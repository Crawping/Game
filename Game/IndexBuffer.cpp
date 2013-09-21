//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

class IndexBuffer::IndexBufferImpl
{
public:

	//////////////////////////////////////////////////////////////////////

	IndexBufferImpl(void *indices, uint numIndices, IndexBuffer::Type type)
	{
		size_t size = (type == IT_UINT16) ? sizeof(uint16) : sizeof(uint32);
		mFormat = (type == IT_UINT16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = numIndices * size;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.StructureByteStride = size;
		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices; 
		DX(D3DDevice->CreateBuffer(&desc, &data, &mBuffer));
	}

	//////////////////////////////////////////////////////////////////////

	~IndexBufferImpl()
	{
		::Release(mBuffer);
	}

	//////////////////////////////////////////////////////////////////////

	void Activate()
	{
		D3DContext->IASetIndexBuffer(mBuffer, mFormat, 0);
	}

	//////////////////////////////////////////////////////////////////////

private:

	DXGI_FORMAT				mFormat;
	ID3D11Buffer *			mBuffer;
};

//////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer()
	: pImpl(null)
{
}

//////////////////////////////////////////////////////////////////////

IndexBuffer::~IndexBuffer()
{
	Delete(pImpl);
}

//////////////////////////////////////////////////////////////////////

IndexBuffer *IndexBuffer::Create(void *indices, uint numIndices, Type type)
{
	IndexBuffer *i = new IndexBuffer();
	i->pImpl = new IndexBufferImpl(indices, numIndices, type);
	return i;
}

//////////////////////////////////////////////////////////////////////

void IndexBuffer::Activate()
{
	assert(pImpl != null);
	pImpl->Activate();
}

//////////////////////////////////////////////////////////////////////
