//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

class Material::MaterialImpl
{
public:

	//////////////////////////////////////////////////////////////////////

	MaterialImpl(PixelShader *pixelShader, VertexShader *vertexShader, Texture *texture = null, BlendMode blendMode = BM_None)
		: mPixelShader(pixelShader)
		, mVertexShader(vertexShader)
		, mTexture(texture)
		, mBlendMode(blendMode)
	{
	}

	//////////////////////////////////////////////////////////////////////

	~MaterialImpl()
	{
		// client deletes these (refcount them?)
	}

	//////////////////////////////////////////////////////////////////////

	void UpdateConstants(uint index, void const *data) const
	{
		mVertexShader->UpdateConstants(index, data);
	}

	//////////////////////////////////////////////////////////////////////

	int GetConstantBufferIndex(char const *name)
	{
		return mVertexShader->GetConstantBufferIndex(name);
	}

	//////////////////////////////////////////////////////////////////////

	void Activate() const
	{
		Graphics::SetBlendMode(mBlendMode);

		mPixelShader->Activate();
		mVertexShader->Activate();
		if(mTexture != null)
		{
			mTexture->Activate();
		}
		else
		{
			//App::Graphics().EnableTextureChannels(0);
		}
	}

	//////////////////////////////////////////////////////////////////////

private:

	BlendMode			mBlendMode;
	PixelShader *		mPixelShader;
	VertexShader *		mVertexShader;
	Texture *			mTexture;
};

//////////////////////////////////////////////////////////////////////

Material::Material()
	: pImpl(null)
{
}

//////////////////////////////////////////////////////////////////////

Material::~Material()
{
	Delete(pImpl);
}

//////////////////////////////////////////////////////////////////////

Material* Material::Create(PixelShader *pixelShader, VertexShader *vertexShader, BlendMode blendMode, Texture *texture)
{
	Material *m = new Material();
	m->pImpl = new MaterialImpl(pixelShader, vertexShader, texture, blendMode);
	return m;
}

//////////////////////////////////////////////////////////////////////

void Material::Activate() const
{
	assert(pImpl != null);
	pImpl->Activate();
}

//////////////////////////////////////////////////////////////////////

int Material::GetConstantBufferIndex(char const *name)
{
	assert(pImpl != null);
	return pImpl->GetConstantBufferIndex(name);
}

//////////////////////////////////////////////////////////////////////

void Material::UpdateConstants(uint index, void const *data) const
{
	assert(pImpl != null);
	pImpl->UpdateConstants(index, data);
}