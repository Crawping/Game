//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class ConstantBuffer : public RefCount
{
public:

	static ConstantBuffer *	Create(size_t size, char const *name);

	string const &			GetName() const;

	// how many variables in this constant buffer
	uint					GetNumVariables() const;

	// get index of a variable by name
	uint					GetIndex(char const *name);	// -1 = not found

	// get size of a variable by index
	size_t					GetSize(uint index);

	// get name of a variable by index
	string const &			GetVariableName(uint index);

	void					Update(void const *data);
	void					Update(void const *data, size_t size);
	void					Activate();

	size_t					GetSize() const;

private:

	ConstantBuffer();
	~ConstantBuffer();

	class ConstantBufferImpl;
	ConstantBufferImpl *pImpl;
};

//////////////////////////////////////////////////////////////////////

