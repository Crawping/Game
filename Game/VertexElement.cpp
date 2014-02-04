//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// MUST BE THE SAME ORDER AS THE VertexElement::FieldIndex enum

VertexElement VertexElement::sAllVertexElements[] =
{
	{	VertexElement::Position2,	"POSITION",	VertexElement::Float,	2	},
	{	VertexElement::Position3,	"POSITION",	VertexElement::Float,	3	},
	{	VertexElement::TexCoord,	"TEXCOORD",	VertexElement::Float,	2	},
	{	VertexElement::Normal,		"NORMAL",	VertexElement::Float,	3	},
	{	VertexElement::Color,		"COLOR",	VertexElement::Byte,	4	}
};

//////////////////////////////////////////////////////////////////////

size_t VertexElement::sDataTypeSize[VertexElement::NumDataTypes] =
{
	sizeof(float),		// Float = 0,
	sizeof(byte)		// Byte = 1,
};

//////////////////////////////////////////////////////////////////////

size_t VertexElement::GetVertexSize(VertexElement::Field vertexField)
{
	size_t s = 0;
	for(uint i=0; i<VertexElement::FI_NumFields; ++i)
	{
		if((vertexField & (1 << i)) != 0)
		{
			VertexElement &ve = VertexElement::sAllVertexElements[i];
			s += VertexElement::sDataTypeSize[ve.mType] * ve.mNumElements;
		}
	}
	return s;
}

//////////////////////////////////////////////////////////////////////

