//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class VertexShader : public RefCount
{
public:

	static VertexShader *	Load(wchar const *filename, VertexElement::Field vertexDescriptor);

	int						GetConstantBufferIndex(char const *name);
	void					UpdateConstants(uint index, void const *data);
	void					Activate();

private:

	VertexShader();
	~VertexShader();

	class VertexShaderImpl;
	VertexShaderImpl *pImpl;
};

//////////////////////////////////////////////////////////////////////

