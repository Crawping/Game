//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////

enum IMC_CommandType : uint16
{
	IMC_Invalid	=	0,
	IMC_Material,					// Activate the material
	IMC_BlendMode,					// Set BlendMode
	IMC_ZBufferMode,
	IMC_Texture,					// Activate the texture
	IMC_Viewport,
	IMC_Constants,					// Set the constants
	IMC_Triangles,					// Draw the triangles
	IMC_Strip,						// Draw the strip
	IMC_Lines,						// Draw the lines
	IMC_LineStrip,					// Draw the linestrip
	IMC_NumCommandTypes
};

//////////////////////////////////////////////////////////////////////

struct IMC_Command
{
	IMC_CommandType	mCommandType;
};

//////////////////////////////////////////////////////////////////////

struct IMC_MaterialCommand : IMC_Command
{
	Material const *mMaterial;
};

//////////////////////////////////////////////////////////////////////

struct IMC_BlendModeCommand : IMC_Command
{
	BlendMode		mBlendMode;
};

//////////////////////////////////////////////////////////////////////

struct IMC_ZBufferModeCommand : IMC_Command
{
	ZBufferMode		mZBufferMode;
};

//////////////////////////////////////////////////////////////////////

struct IMC_TextureCommand : IMC_Command
{
	Texture const *mTexture;
};

//////////////////////////////////////////////////////////////////////

struct IMC_ViewportCommand : IMC_Command
{
	int		mLeft;
	int		mTop;
	int		mRight;
	int		mBottom;
};

//////////////////////////////////////////////////////////////////////

struct IMC_ConstCommand : IMC_Command
{
	size_t			mSize;		// data follows
};

//////////////////////////////////////////////////////////////////////

struct IMC_DrawCommand : IMC_Command	// for triangles, strips and lines
{
	uint32			mVertexBase;
	uint32			mVertexCount;
};

//////////////////////////////////////////////////////////////////////

ImmediateContext::ImmediateContext(VertexElement::Field vertexFieldMask, uint maxVerts, size_t constantBufferSize)
{
	mMaxVerts = maxVerts;
	mVertexSize = VertexElement::GetVertexSize(vertexFieldMask);

	for(uint i=0; i<kNumBuffers; ++i)
	{
		mVertexBuffer[i] = VertexBuffer::Create(mMaxVerts, vertexFieldMask, null);
		mConstantBuffer[i] = ConstantBuffer::Create(constantBufferSize, "");
	}

	mConstantBufferSize = constantBufferSize;
	mCommandBufferSize = 65536;
	mCommandBuffer = new byte[mCommandBufferSize];
	mCommandBufferPointer = mCommandBuffer;
	mBufferIndex = 0;
}

//////////////////////////////////////////////////////////////////////

ImmediateContext::~ImmediateContext()
{
	for(uint i=0; i<kNumBuffers; ++i)
	{
		::Release(mVertexBuffer[i]);
		::Release(mConstantBuffer[i]);
	}
	DeleteArray(mCommandBuffer);
}

//////////////////////////////////////////////////////////////////////

ImmediateContext *ImmediateContext::Create(VertexElement::Field vertexFieldMask, uint maxVerts, size_t constantBufferSize)
{
	return new ImmediateContext(vertexFieldMask, maxVerts, constantBufferSize);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::Begin()
{
	mCommandBufferPointer = mCommandBuffer;
	mCommandBufferEnd = mCommandBuffer + mCommandBufferSize;
	mVertexBufferBase = mVertexBufferPointer = mVertexBuffer[mBufferIndex]->Map();
	mVertexBufferEnd = mVertexBufferPointer + mVertexSize * mMaxVerts;
	mVertexIndex = 0;
	mNumCommands = 0;
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetMaterial(Material const *material)
{
	if(mCommandBufferPointer + sizeof(IMC_MaterialCommand) < mCommandBufferEnd)
	{
		IMC_MaterialCommand *cmd = reinterpret_cast<IMC_MaterialCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_Material;
		cmd->mMaterial = material;
		mCommandBufferPointer += sizeof(IMC_MaterialCommand);
		++mNumCommands;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetBlendMode(BlendMode blendMode)
{
	if(mCommandBufferPointer + sizeof(IMC_BlendModeCommand) < mCommandBufferEnd)
	{
		IMC_BlendModeCommand *cmd = reinterpret_cast<IMC_BlendModeCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_BlendMode;
		cmd->mBlendMode = blendMode;
		mCommandBufferPointer += sizeof(IMC_BlendModeCommand);
		++mNumCommands;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetZBufferMode(ZBufferMode mode)
{
	if(mCommandBufferPointer + sizeof(IMC_ZBufferModeCommand) < mCommandBufferEnd)
	{
		IMC_ZBufferModeCommand *cmd = reinterpret_cast<IMC_ZBufferModeCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_ZBufferMode;
		cmd->mZBufferMode = mode;
		mCommandBufferPointer += sizeof(IMC_ZBufferModeCommand);
		++mNumCommands;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetViewport(int left, int top, int right, int bottom)
{
	if(mCommandBufferPointer + sizeof(IMC_ViewportCommand) < mCommandBufferEnd)
	{
		IMC_ViewportCommand *cmd = reinterpret_cast<IMC_ViewportCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_Viewport;
		cmd->mLeft = left;
		cmd->mRight = right;
		cmd->mTop = top;
		cmd->mBottom = bottom;
		mCommandBufferPointer += sizeof(IMC_ViewportCommand);
		++mNumCommands;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetTexture(Texture const *texture)
{
	if(mCommandBufferPointer + sizeof(IMC_TextureCommand) < mCommandBufferEnd)
	{
		IMC_TextureCommand *cmd = reinterpret_cast<IMC_TextureCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_Texture;
		cmd->mTexture = texture;
		mCommandBufferPointer += sizeof(IMC_TextureCommand);
		++mNumCommands;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetConstants(void const *data, size_t size)
{
	if(mCommandBufferPointer + sizeof(IMC_ConstCommand) < mCommandBufferEnd)
	{
		IMC_ConstCommand *cmd = reinterpret_cast<IMC_ConstCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_Constants;
		cmd->mSize = size;
		byte *dstData = reinterpret_cast<byte *>(cmd + 1);
		memcpy(dstData, data, size); 
		mCommandBufferPointer += sizeof(IMC_ConstCommand) + size;
		++mNumCommands;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::BeginTriangles()
{
	if(mCommandBufferPointer + sizeof(IMC_DrawCommand) < mCommandBufferEnd)
	{
		IMC_DrawCommand *cmd = reinterpret_cast<IMC_DrawCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_Triangles;
		cmd->mVertexBase = mVertexIndex;
		cmd->mVertexCount = 0;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::BeginStrip()
{
	if(mCommandBufferPointer + sizeof(IMC_DrawCommand) < mCommandBufferEnd)
	{
		IMC_DrawCommand *cmd = reinterpret_cast<IMC_DrawCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_Strip;
		cmd->mVertexBase = mVertexIndex;
		cmd->mVertexCount = 0;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::BeginLines()
{
	if(mCommandBufferPointer + sizeof(IMC_DrawCommand) < mCommandBufferEnd)
	{
		IMC_DrawCommand *cmd = reinterpret_cast<IMC_DrawCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_Lines;
		cmd->mVertexBase = mVertexIndex;
		cmd->mVertexCount = 0;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::BeginLineStrip()
{
	if(mCommandBufferPointer + sizeof(IMC_DrawCommand) < mCommandBufferEnd)
	{
		IMC_DrawCommand *cmd = reinterpret_cast<IMC_DrawCommand *>(mCommandBufferPointer);
		cmd->mCommandType = IMC_LineStrip;
		cmd->mVertexBase = mVertexIndex;
		cmd->mVertexCount = 0;
	}
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::BeginVertex()
{
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetPosition3(float x, float y, float z)
{
	float *p = (float *)mVertexBufferPointer;
	p[0] = x;
	p[1] = y;
	p[2] = z;
	mVertexBufferPointer += sizeof(Vec3);
	assert(mVertexBufferPointer < mVertexBufferEnd);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetPosition3(Vec4f pos)
{
	*reinterpret_cast<Vec3 *>(mVertexBufferPointer) = *reinterpret_cast<Vec3 *>(&pos);
	mVertexBufferPointer += sizeof(Vec3);
	assert(mVertexBufferPointer < mVertexBufferEnd);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetPosition2(Vec2 const &pos)
{
	*reinterpret_cast<Vec2 *>(mVertexBufferPointer) = pos;
	mVertexBufferPointer += sizeof(Vec2);
	assert(mVertexBufferPointer < mVertexBufferEnd);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetPosition2(float x, float y)
{
	*reinterpret_cast<Vec2 *>(mVertexBufferPointer) = Vec2(x, y);
	mVertexBufferPointer += sizeof(Vec2);
	assert(mVertexBufferPointer < mVertexBufferEnd);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetTexCoord(float u, float v)
{
	*reinterpret_cast<Vec2 *>(mVertexBufferPointer) = Vec2(u, v);
	mVertexBufferPointer += sizeof(Vec2);
	assert(mVertexBufferPointer < mVertexBufferEnd);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetTexCoord(Vec2 const &uv)
{
	*reinterpret_cast<Vec2 *>(mVertexBufferPointer) = uv;
	mVertexBufferPointer += sizeof(Vec2);
	assert(mVertexBufferPointer < mVertexBufferEnd);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetNormal(float x, float y, float z)
{
	Vec3 &p = *reinterpret_cast<Vec3 *>(mVertexBufferPointer);
	p.x = x;
	p.y = y;
	p.z = z;
	mVertexBufferPointer += sizeof(Vec3);
	assert(mVertexBufferPointer < mVertexBufferEnd);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetNormal(Vec4f normal)
{
	Vec3 &p = *reinterpret_cast<Vec3 *>(mVertexBufferPointer);
	p.x = GetX(normal);
	p.y = GetY(normal);
	p.z = GetZ(normal);
	mVertexBufferPointer += sizeof(Vec3);
	assert(mVertexBufferPointer < mVertexBufferEnd);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::SetColor(Color color)
{
	*reinterpret_cast<Color *>(mVertexBufferPointer) = color;
	mVertexBufferPointer += sizeof(Color);
	assert(mVertexBufferPointer < mVertexBufferEnd);
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::EndVertex()
{
	++mVertexIndex;
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::EndTriangles()
{
	IMC_DrawCommand *cmd = reinterpret_cast<IMC_DrawCommand *>(mCommandBufferPointer);
	cmd->mVertexCount = mVertexIndex - cmd->mVertexBase;
	mCommandBufferPointer += sizeof(IMC_DrawCommand);
	++mNumCommands;
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::EndStrip()
{
	IMC_DrawCommand *cmd = reinterpret_cast<IMC_DrawCommand *>(mCommandBufferPointer);
	cmd->mVertexCount = mVertexIndex - cmd->mVertexBase;
	mCommandBufferPointer += sizeof(IMC_DrawCommand);
	++mNumCommands;
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::EndLines()
{
	IMC_DrawCommand *cmd = reinterpret_cast<IMC_DrawCommand *>(mCommandBufferPointer);
	cmd->mVertexCount = mVertexIndex - cmd->mVertexBase;
	mCommandBufferPointer += sizeof(IMC_DrawCommand);
	++mNumCommands;
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::EndLineStrip()
{
	IMC_DrawCommand *cmd = reinterpret_cast<IMC_DrawCommand *>(mCommandBufferPointer);
	cmd->mVertexCount = mVertexIndex - cmd->mVertexBase;
	mCommandBufferPointer += sizeof(IMC_DrawCommand);
	++mNumCommands;
}

//////////////////////////////////////////////////////////////////////

void ImmediateContext::End()
{
	mVertexBuffer[mBufferIndex]->UnMap();
	mVertexBuffer[mBufferIndex]->Activate();
	mConstantBuffer[mBufferIndex]->Activate();

	byte *cmd = mCommandBuffer;
	for(uint i=0; i<mNumCommands; ++i)
	{
		switch((reinterpret_cast<IMC_Command *>(cmd))->mCommandType)
		{
		case IMC_Material:
			{
				IMC_MaterialCommand *matCommand = reinterpret_cast<IMC_MaterialCommand *>(cmd);
				matCommand->mMaterial->Activate();
				mCurrentMaterial = matCommand->mMaterial;
				cmd += sizeof(IMC_MaterialCommand);
			}
			break;

		case IMC_Texture:
			{
				IMC_TextureCommand *texCommand = reinterpret_cast<IMC_TextureCommand *>(cmd);
				texCommand->mTexture->Activate();
				cmd += sizeof(IMC_TextureCommand);
			}
			break;

		case IMC_Viewport:
			{
				IMC_ViewportCommand *vp = reinterpret_cast<IMC_ViewportCommand *>(cmd);
				Graphics::SetViewport(vp->mLeft, vp->mTop, vp->mRight, vp->mBottom);
				cmd += sizeof(IMC_ViewportCommand);
			}
			break;

		case IMC_BlendMode:
			{
				IMC_BlendModeCommand *blendCommand = reinterpret_cast<IMC_BlendModeCommand *>(cmd);
				Graphics::SetBlendMode(blendCommand->mBlendMode);
				cmd += sizeof(IMC_BlendModeCommand);
			}
			break;

		case IMC_ZBufferMode:
			{
				IMC_ZBufferModeCommand *command = reinterpret_cast<IMC_ZBufferModeCommand *>(cmd);
				Graphics::SetZBufferMode(command->mZBufferMode);
				cmd += sizeof(IMC_ZBufferModeCommand);
			}
			break;

		case IMC_Constants:
			{
				IMC_ConstCommand *constCommand = reinterpret_cast<IMC_ConstCommand *>(cmd);
				mCurrentMaterial->UpdateConstants(0, reinterpret_cast<void *>(constCommand + 1));
				cmd += sizeof(IMC_ConstCommand) + constCommand->mSize;
			}
			break;

		case IMC_Triangles:
			{
				IMC_DrawCommand *drawCommand = reinterpret_cast<IMC_DrawCommand *>(cmd);
				Graphics::Draw(MeshType::MT_Triangles, drawCommand->mVertexBase, drawCommand->mVertexCount);
				cmd += sizeof(IMC_DrawCommand);
			}
			break;

		case IMC_Strip:
			{
				IMC_DrawCommand *drawCommand = reinterpret_cast<IMC_DrawCommand *>(cmd);
				Graphics::Draw(MeshType::MT_Strip, drawCommand->mVertexBase, drawCommand->mVertexCount);
				cmd += sizeof(IMC_DrawCommand);
			}
			break;

		case IMC_Lines:
			{
				IMC_DrawCommand *drawCommand = reinterpret_cast<IMC_DrawCommand *>(cmd);
				Graphics::Draw(MeshType::MT_Lines, drawCommand->mVertexBase, drawCommand->mVertexCount);
				cmd += sizeof(IMC_DrawCommand);
			}
			break;

		case IMC_LineStrip:
			{
				IMC_DrawCommand *drawCommand = reinterpret_cast<IMC_DrawCommand *>(cmd);
				Graphics::Draw(MeshType::MT_LineStrip, drawCommand->mVertexBase, drawCommand->mVertexCount);
				cmd += sizeof(IMC_DrawCommand);
			}
			break;

		default:
			assert(false && "Unknown Immediate Mode Context command encountered...");
			i = mNumCommands;	// break out of for loop
			break;
		}
	}

	mBufferIndex = (++mBufferIndex) % kNumBuffers;
}