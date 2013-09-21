//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Texture;
class PixelShader;
class VertexShader;

//////////////////////////////////////////////////////////////////////

enum BlendMode : uint32
{
	BM_None				=	0,
	BM_Additive			=	1,
	BM_Modulate			=	2,
	BM_NumBlendModes	=	3
};

enum ZBufferMode : uint32
{
	ZB_Enable			=	0,
	ZB_Disable			=	1,
	ZB_NumZBufferModes	=	2
};

//////////////////////////////////////////////////////////////////////

class Material : public RefCount
{
public:
	static Material *	Create(PixelShader *pixelShader, VertexShader *vertexShader, BlendMode blendMode, Texture * texture = null);
	int					GetConstantBufferIndex(char const *name);
	void				UpdateConstants(uint index, void const *data) const;
	void				Activate() const;

private:

	Material();
	~Material();

	class MaterialImpl;
	MaterialImpl *pImpl;
};

