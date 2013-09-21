//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

struct VertexElement
{
	enum DataType
	{
		Float = 0,
		Byte = 1,
		NumDataTypes = 2
	};

	enum FieldIndex : uint32
	{
		FI_Position2	= 0,
		FI_Position3	= 1,
		FI_TexCoord		= 2,
		FI_Normal		= 3,
		FI_Color		= 4,
		FI_NumFields	= 5
	};

	enum FieldMask : uint32
	{
		Position2	= 1 << FI_Position2,
		Position3	= 1 << FI_Position3,
		TexCoord	= 1 << FI_TexCoord,
		Normal		= 1 << FI_Normal,
		Color		= 1 << FI_Color
	};

	typedef uint32 Field;

	FieldMask		mBitField;
	char const *	mName;
	DataType		mType;
	uint32			mNumElements;

	static size_t			GetVertexSize(VertexElement::Field vertexField);

	static VertexElement	sAllVertexElements[];
	static size_t			sDataTypeSize[];
};

//////////////////////////////////////////////////////////////////////

