//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class ModelNode
{
public:

	uint		AddChild(ModelNode *child);
	void		SetIndex(uint index);
	uint		GetIndex() const;
	void		RemoveChild(uint index);
	uint		GetNumChildren();
	void		AddMesh(Mesh *mesh);
	void		SetTransform(Matrix const &transformMatrix);
	ModelNode *	GetChild(uint index);

private:

	void		Release();

	vector<ModelNode *>	mChildren;
	vector<Mesh *>		mMeshes;
	Matrix				mTransform;
	uint32				mIndex;

	void		Draw(Matrix const &modelMatrix, Camera &camera);

	friend class Model;
};

