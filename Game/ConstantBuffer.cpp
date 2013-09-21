//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

#define USE_MAP

class ConstantBuffer::ConstantBufferImpl
{
public:

	//////////////////////////////////////////////////////////////////////

	ConstantBufferImpl(size_t size, char const *name)
		: mSize(size)
		, mName(name)
		, mBuffer(null)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
#if defined(USE_MAP)
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
#else
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
#endif
		bd.ByteWidth = size;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		DX(D3DDevice->CreateBuffer(&bd, NULL, &mBuffer));
	}

	//////////////////////////////////////////////////////////////////////

	~ConstantBufferImpl()
	{
		::Release(mBuffer);
	}

	//////////////////////////////////////////////////////////////////////

	string const &GetName() const
	{
		return mName;
	}

	//////////////////////////////////////////////////////////////////////

	size_t GetSize() const throw()
	{
		return mSize;
	}

	//////////////////////////////////////////////////////////////////////

	void Update(void const *data)
	{
#if defined(USE_MAP)
		D3D11_MAPPED_SUBRESOURCE sr;
		DX(D3DContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sr));
		memcpy(sr.pData, data, mSize);
		D3DContext->Unmap(mBuffer, 0);
#else
		D3DContext->UpdateSubresource(mBuffer, 0, null, data, mSize, 0);
#endif
	}

	//////////////////////////////////////////////////////////////////////

	void Update(void const *data, size_t size)
	{
#if defined(USE_MAP)
		D3D11_MAPPED_SUBRESOURCE sr;
		DX(D3DContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sr));
		memcpy(sr.pData, data, size);
		D3DContext->Unmap(mBuffer, 0);
#else
		D3DContext->UpdateSubresource(mBuffer, 0, null, data, size, 0);
#endif
	}

	//////////////////////////////////////////////////////////////////////
	// TODO: allow CB set by index

	void Activate()
	{
		D3DContext->VSSetConstantBuffers(0, 1, &mBuffer);
	}

	//////////////////////////////////////////////////////////////////////

private:

	size_t			mSize;
	string			mName;
	ID3D11Buffer *	mBuffer;
};

//////////////////////////////////////////////////////////////////////

ConstantBuffer::ConstantBuffer()
	: pImpl(null)
{
}

//////////////////////////////////////////////////////////////////////

ConstantBuffer::~ConstantBuffer()
{
	Delete(pImpl);
}

//////////////////////////////////////////////////////////////////////

ConstantBuffer *ConstantBuffer::Create(size_t size, char const *name)
{
	ConstantBuffer *c = new ConstantBuffer();
	c->pImpl = new ConstantBufferImpl(size, name);
	return c;
}

//////////////////////////////////////////////////////////////////////

string const &ConstantBuffer::GetName() const
{
	assert(pImpl != null);
	return pImpl->GetName();
}

//////////////////////////////////////////////////////////////////////

size_t ConstantBuffer::GetSize() const
{
	assert(pImpl != null);
	return pImpl->GetSize();
}

//////////////////////////////////////////////////////////////////////

void ConstantBuffer::Update(void const *data)
{
	assert(pImpl != null);
	pImpl->Update(data);
}

//////////////////////////////////////////////////////////////////////

void ConstantBuffer::Update(void const *data, size_t size)
{
	assert(pImpl != null);
	pImpl->Update(data, size);
}

//////////////////////////////////////////////////////////////////////

void ConstantBuffer::Activate()
{
	assert(pImpl != null);
	pImpl->Activate();
}

