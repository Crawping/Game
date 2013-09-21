//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

enum MeshType
{
	MT_Triangles = 0,
	MT_Strip = 1,
	MT_Lines = 2,
	MT_LineStrip = 3,
	MT_NumTypes = 4
};

//////////////////////////////////////////////////////////////////////

class Mesh : public RefCount
{
public:

	static Mesh *	Create(MeshType type, uint vertexBase, uint indexBase, uint indexCount, Material *material);
	Material *		GetMaterial() const;
	void			Draw();

private:

	Mesh();
	~Mesh();

	MeshType	mType;
	uint32		mVertexBase;
	uint32		mIndexBase;
	uint32		mIndexCount;
	Material *	mMaterial;

};

//////////////////////////////////////////////////////////////////////
