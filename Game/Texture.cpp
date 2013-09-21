//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DDSTextureLoader.h"

//////////////////////////////////////////////////////////////////////

class  Texture::TextureImpl
{
public:

	//////////////////////////////////////////////////////////////////////

	TextureImpl(wchar const *filename)
		: mTexture(null)
//		, mSurface(null)
		, mShaderResourceView(null)
		, mSamplerState(null)
	{
		DX(DirectX::CreateDDSTextureFromFile(D3DDevice, filename, (ID3D11Resource **)&mTexture, &mShaderResourceView));
		Finalize();
	}

	//////////////////////////////////////////////////////////////////////
	// Multiple texture channels
	// bool renderTarget
	// bool allowGDIAccess
	// bool allowCPUAccess
	// format

	TextureImpl(uint width, uint height, Color color, bool createMips = false)
		: mTexture(null)
//		, mSurface(null)
		, mShaderResourceView(null)
		, mSamplerState(null)
	{
		ZeroMemory(&mDesc, sizeof(mDesc));
		mDesc.Width = width;
		mDesc.Height = height;
		mDesc.MipLevels = createMips ? 0 : 1;
		mDesc.ArraySize = 1;
		mDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		mDesc.SampleDesc.Count = 1;
		mDesc.SampleDesc.Quality = 0;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
		mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (createMips ? D3D11_BIND_RENDER_TARGET : 0);
		mDesc.CPUAccessFlags = 0;
		mDesc.MiscFlags = createMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		uint32 *pixels = new uint32[width * height];
		for(uint y = 0; y < width * height; ++y)
		{
			pixels[y] = color;
		}
		D3D11_SUBRESOURCE_DATA data[1];
		data[0].pSysMem = (void *)pixels;
		data[0].SysMemPitch = width * 4;
		data[0].SysMemSlicePitch = 0;
		D3DDevice->CreateTexture2D(&mDesc, data, &mTexture);
		DeleteArray(pixels);
		D3DDevice->CreateShaderResourceView(mTexture, null, &mShaderResourceView);
		Finalize();
	}

	//////////////////////////////////////////////////////////////////////

	void Finalize()
	{
		mTexture->GetDesc(&mDesc);
		//mTexture->QueryInterface(__uuidof(IDXGISurface1), (void **)(&mSurface));
		CD3D11_SAMPLER_DESC samplerDesc(D3D11_DEFAULT);
		D3DDevice->CreateSamplerState(&samplerDesc, &mSamplerState);
	}

	//////////////////////////////////////////////////////////////////////

	~TextureImpl()
	{
		Destroy();
	}

	//////////////////////////////////////////////////////////////////////

	void Destroy()
	{
		::Release(mSamplerState);
		::Release(mShaderResourceView);
//		::Release(mSurface);
		::Release(mTexture);
	}

	//////////////////////////////////////////////////////////////////////

	void Activate()
	{
		D3DContext->PSSetShaderResources(0, 1, &mShaderResourceView);
		D3DContext->PSSetSamplers(0, 1, &mSamplerState);
	}

	//////////////////////////////////////////////////////////////////////

	uint Width() const
	{
		return mDesc.Width;
	}

	//////////////////////////////////////////////////////////////////////

	uint Height() const
	{
		return mDesc.Height;
	}

	//////////////////////////////////////////////////////////////////////

private:

//	IDXGISurface1 *				mSurface;		// In case we ever want to use GDI
	ID3D11Texture2D *			mTexture;
	ID3D11ShaderResourceView *	mShaderResourceView;
	ID3D11SamplerState *		mSamplerState;
	D3D11_TEXTURE2D_DESC		mDesc;
};

//////////////////////////////////////////////////////////////////////

Texture::Texture()
	: pImpl(null)
{
}

//////////////////////////////////////////////////////////////////////

Texture::~Texture()
{
	Delete(pImpl);
}

//////////////////////////////////////////////////////////////////////

uint Texture::Width() const
{
	assert(pImpl != null);
	return pImpl->Width();
}

//////////////////////////////////////////////////////////////////////

uint Texture::Height() const
{
	assert(pImpl != null);
	return pImpl->Height();
}

//////////////////////////////////////////////////////////////////////

Texture *Texture::Create(uint width, uint height, Color color, bool createMips)
{
	Texture *t = new Texture();
	t->pImpl = new TextureImpl(width, height, color, createMips);
	return t;
}

//////////////////////////////////////////////////////////////////////

Texture *Texture::Load(wchar const *filename)
{
	Texture *t = new Texture();
	t->pImpl = new TextureImpl(filename);
	return t;
}

//////////////////////////////////////////////////////////////////////

void Texture::Activate() const
{
	assert(pImpl != null);
	pImpl->Activate();
}

//////////////////////////////////////////////////////////////////////

