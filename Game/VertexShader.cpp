//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

class VertexShader::VertexShaderImpl
{
public:

	//////////////////////////////////////////////////////////////////////

	VertexShaderImpl(wchar const *filename, VertexElement::Field vertexDescriptor)
		: mVertexShader(null)
		, mInputLayout(null)
	{
		ID3DBlob *blob;
		DX(D3DReadFileToBlob(Format(L"%s.cso", filename).c_str(), &blob));

		//TRACE(L"Vertex Shader: %s\n", filename);

		uint index = 0;
		for(uint i=0; i<VertexElement::FI_NumFields; ++i)
		{
			if((vertexDescriptor & (1 << i)) != 0)
			{
				D3D11_INPUT_ELEMENT_DESC &d = sIEDesc[index];
				VertexElement &e = VertexElement::sAllVertexElements[i];
				d.SemanticName = e.mName;
				d.SemanticIndex = 0;
				d.Format = sD3DVertFormat[e.mType][e.mNumElements - 1];
				d.InputSlot = 0;
				d.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				d.InstanceDataStepRate = 0;
				++index;

				assert(d.Format != DXGI_FORMAT_UNKNOWN);
			}
		}
		DX(D3DDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), null, &mVertexShader));
		DX(D3DDevice->CreateInputLayout(sIEDesc, index, blob->GetBufferPointer(), blob->GetBufferSize(), &mInputLayout));

		// Reflect shader info
		ID3D11ShaderReflection* pVertexShaderReflection = NULL; 
		DX(D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &pVertexShaderReflection));

		D3D11_SHADER_DESC shaderDesc;
		DX(pVertexShaderReflection->GetDesc(&shaderDesc));

		//TRACE("\tInput parameters: %d\n", shaderDesc.InputParameters);

		for(uint i=0; i<shaderDesc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC desc;
			pVertexShaderReflection->GetInputParameterDesc(i, &desc);

		//	string componentType;
		//	switch (desc.ComponentType)
		//	{
		//	case D3D_REGISTER_COMPONENT_UINT32:
		//		componentType = "uint32";
		//		break;
		//	case D3D_REGISTER_COMPONENT_SINT32:
		//		componentType = "int32";
		//		break;
		//	case D3D_REGISTER_COMPONENT_FLOAT32:
		//		componentType = "float";
		//		break;
		//	default:
		//		componentType = "unknown";
		//		break;
		//	}
		//	TRACE("\t\tInput parameter %d: %s, %d, %s, %02x\n", i, desc.SemanticName, desc.SemanticIndex, componentType.c_str(), desc.Mask);
		}

		for(uint i=0; i<shaderDesc.ConstantBuffers; ++i)
		{
			ID3D11ShaderReflectionConstantBuffer *buffer = pVertexShaderReflection->GetConstantBufferByIndex(i);
			D3D11_SHADER_BUFFER_DESC desc;
			DX(buffer->GetDesc(&desc));
		//	TRACE("\tConstant buffer %s: %d\n", desc.Name, desc.Size);
		//	for(uint j=0; j<desc.Variables; ++j)
		//	{
		//		ID3D11ShaderReflectionVariable *var = buffer->GetVariableByIndex(j);
		//		D3D11_SHADER_VARIABLE_DESC varDesc;
		//		DX(var->GetDesc(&varDesc));

		//		ID3D11ShaderReflectionType *varType = var->GetType();
		//		D3D11_SHADER_TYPE_DESC typeDesc;
		//		DX(varType->GetDesc(&typeDesc));

		//		string type;
		//		switch (typeDesc.Class)
		//		{
		//		case D3D_SVC_SCALAR:
		//			type = "SCALAR";
		//			break;
		//		case D3D_SVC_VECTOR:
		//			type = "VECTOR";
		//			break;
		//		case D3D_SVC_MATRIX_ROWS:
		//			type = "MATRIX_ROWS";
		//			break;
		//		case D3D_SVC_MATRIX_COLUMNS:
		//			type = "MATRIX_COLUMNS";
		//			break;
		//		case D3D_SVC_OBJECT:
		//			type = "OBJECT";
		//			break;
		//		case D3D_SVC_STRUCT:
		//			type = "STRUCT";
		//			break;
		//		case D3D_SVC_INTERFACE_CLASS:
		//			type = "INTERFACE_CLASS";
		//			break;
		//		case D3D_SVC_INTERFACE_POINTER:
		//			type = "INTERFACE_POINTER";
		//			break;
		//		default:
		//			break;
		//		}

		//		string vtype;

		//		switch (typeDesc.Type)
		//		{
		//		case D3D_SVT_VOID:
		//			vtype = "VOID";
		//			break;
		//		case D3D_SVT_BOOL:
		//			vtype = "BOOL";
		//			break;
		//		case D3D_SVT_INT:
		//			vtype = "INT";
		//			break;
		//		case D3D_SVT_FLOAT:
		//			vtype = "FLOAT";
		//			break;
		//		case D3D_SVT_STRING:
		//			vtype = "STRING";
		//			break;
		//		case D3D_SVT_TEXTURE:
		//			vtype = "TEXTURE";
		//			break;
		//		case D3D_SVT_TEXTURE1D:
		//			vtype = "TEXTURE1D";
		//			break;
		//		case D3D_SVT_TEXTURE2D:
		//			vtype = "TEXTURE2D";
		//			break;
		//		case D3D_SVT_TEXTURE3D:
		//			vtype = "TEXTURE3D";
		//			break;
		//		case D3D_SVT_TEXTURECUBE:
		//			vtype = "TEXTURECUBE";
		//			break;
		//		case D3D_SVT_SAMPLER:
		//			vtype = "SAMPLER";
		//			break;
		//		case D3D_SVT_PIXELSHADER:
		//			vtype = "PIXELSHADER";
		//			break;
		//		case D3D_SVT_VERTEXSHADER:
		//			vtype = "VERTEXSHADER";
		//			break;
		//		case D3D_SVT_UINT:
		//			vtype = "UINT";
		//			break;
		//		case D3D_SVT_UINT8:
		//			vtype = "UINT8";
		//			break;
		//		case D3D_SVT_GEOMETRYSHADER:
		//			vtype = "GEOMETRYSHADER";
		//			break;
		//		case D3D_SVT_RASTERIZER:
		//			vtype = "RASTERIZER";
		//			break;
		//		case D3D_SVT_DEPTHSTENCIL:
		//			vtype = "DEPTHSTENCIL";
		//			break;
		//		case D3D_SVT_BLEND:
		//			vtype = "BLEND";
		//			break;
		//		case D3D_SVT_BUFFER:
		//			vtype = "BUFFER";
		//			break;
		//		case D3D_SVT_CBUFFER:
		//			vtype = "CBUFFER";
		//			break;
		//		case D3D_SVT_TBUFFER:
		//			vtype = "TBUFFER";
		//			break;
		//		case D3D_SVT_TEXTURE1DARRAY:
		//			vtype = "TEXTURE1DARRAY";
		//			break;
		//		case D3D_SVT_TEXTURE2DARRAY:
		//			vtype = "TEXTURE2DARRAY";
		//			break;
		//		case D3D_SVT_RENDERTARGETVIEW:
		//			vtype = "RENDERTARGETVIEW";
		//			break;
		//		case D3D_SVT_DEPTHSTENCILVIEW:
		//			vtype = "DEPTHSTENCILVIEW";
		//			break;
		//		case D3D_SVT_TEXTURE2DMS:
		//			vtype = "TEXTURE2DMS";
		//			break;
		//		case D3D_SVT_TEXTURE2DMSARRAY:
		//			vtype = "TEXTURE2DMSARRAY";
		//			break;
		//		case D3D_SVT_TEXTURECUBEARRAY:
		//			vtype = "TEXTURECUBEARRAY";
		//			break;
		//		case D3D_SVT_HULLSHADER:
		//			vtype = "HULLSHADER";
		//			break;
		//		case D3D_SVT_DOMAINSHADER:
		//			vtype = "DOMAINSHADER";
		//			break;
		//		case D3D_SVT_INTERFACE_POINTER:
		//			vtype = "INTERFACE_POINTER";
		//			break;
		//		case D3D_SVT_COMPUTESHADER:
		//			vtype = "COMPUTESHADER";
		//			break;
		//		case D3D_SVT_DOUBLE:
		//			vtype = "DOUBLE";
		//			break;
		//		case D3D_SVT_RWTEXTURE1D:
		//			vtype = "RWTEXTURE1D";
		//			break;
		//		case D3D_SVT_RWTEXTURE1DARRAY:
		//			vtype = "RWTEXTURE1DARRAY";
		//			break;
		//		case D3D_SVT_RWTEXTURE2D:
		//			vtype = "RWTEXTURE2D";
		//			break;
		//		case D3D_SVT_RWTEXTURE2DARRAY:
		//			vtype = "RWTEXTURE2DARRAY";
		//			break;
		//		case D3D_SVT_RWTEXTURE3D:
		//			vtype = "RWTEXTURE3D";
		//			break;
		//		case D3D_SVT_RWBUFFER:
		//			vtype = "RWBUFFER";
		//			break;
		//		case D3D_SVT_BYTEADDRESS_BUFFER:
		//			vtype = "BYTEADDRESS_BUFFER";
		//			break;
		//		case D3D_SVT_RWBYTEADDRESS_BUFFER:
		//			vtype = "RWBYTEADDRESS_BUFFER";
		//			break;
		//		case D3D_SVT_STRUCTURED_BUFFER:
		//			vtype = "STRUCTURED_BUFFER";
		//			break;
		//		case D3D_SVT_RWSTRUCTURED_BUFFER:
		//			vtype = "RWSTRUCTURED_BUFFER";
		//			break;
		//		case D3D_SVT_APPEND_STRUCTURED_BUFFER:
		//			vtype = "APPEND_STRUCTURED_BUFFER";
		//			break;
		//		case D3D_SVT_CONSUME_STRUCTURED_BUFFER:
		//			vtype = "CONSUME_STRUCTURED_BUFFER";
		//			break;
		//		default:
		//			break;
		//		}

		//		TRACE("\t\t%s %s[%d,%d] (%s, %d bytes @ %d)\n", ToLower(vtype).c_str(), varDesc.Name, typeDesc.Columns, typeDesc.Rows, type.c_str(), varDesc.Size, varDesc.StartOffset);
		//	}
		//	TRACE("\n");
			mConstantBuffers.push_back(ConstantBuffer::Create(desc.Size, desc.Name));
		}

	}

	//////////////////////////////////////////////////////////////////////

	~VertexShaderImpl()
	{
		::Release(mInputLayout);
		::Release(mVertexShader);
		for(auto v : mConstantBuffers)
		{
			v->Release();
		}
		mConstantBuffers.clear();
	}

	//////////////////////////////////////////////////////////////////////

	void Activate()
	{
		D3DContext->IASetInputLayout(mInputLayout);
		D3DContext->VSSetShader(mVertexShader, null, 0);
		for(auto v : mConstantBuffers)
		{
			v->Activate();
		}
	}

	//////////////////////////////////////////////////////////////////////

	int GetConstantBufferIndex(char const *name)
	{
		uint i=0;
		for(auto v : mConstantBuffers)
		{
			if(v->GetName().compare(name) == 0)
			{
				return i;
			}
			++i;
		}
		return -1;
	}

	//////////////////////////////////////////////////////////////////////

	void UpdateConstants(uint index, void const *data)
	{
		mConstantBuffers[index]->Update(data);
	}

	//////////////////////////////////////////////////////////////////////

private:

	vector<ConstantBuffer *>	mConstantBuffers;
	ID3D11VertexShader *		mVertexShader;
	ID3D11InputLayout *			mInputLayout;

	static D3D11_INPUT_ELEMENT_DESC	sIEDesc[VertexElement::FI_NumFields];
	static DXGI_FORMAT sD3DVertFormat[VertexElement::DataType::NumDataTypes][4];
};

//////////////////////////////////////////////////////////////////////

D3D11_INPUT_ELEMENT_DESC VertexShader::VertexShaderImpl::sIEDesc[VertexElement::FI_NumFields];

DXGI_FORMAT VertexShader::VertexShaderImpl::sD3DVertFormat[VertexElement::DataType::NumDataTypes][4] =
{
	{ DXGI_FORMAT_R32_FLOAT,	DXGI_FORMAT_R32G32_FLOAT,	DXGI_FORMAT_R32G32B32_FLOAT,	DXGI_FORMAT_R32G32B32A32_FLOAT	},
	{ DXGI_FORMAT_UNKNOWN,		DXGI_FORMAT_UNKNOWN,		DXGI_FORMAT_UNKNOWN,			DXGI_FORMAT_R8G8B8A8_UNORM		}
};

//////////////////////////////////////////////////////////////////////

VertexShader::VertexShader()
	: pImpl(null)
{
}

//////////////////////////////////////////////////////////////////////

VertexShader::~VertexShader()
{
	Delete(pImpl);
}

//////////////////////////////////////////////////////////////////////

VertexShader *VertexShader::Load(wchar const *filename, VertexElement::Field vertexDescriptor)
{
	VertexShader *v = new VertexShader();
	v->pImpl = new VertexShaderImpl(filename, vertexDescriptor);
	return v;
}

//////////////////////////////////////////////////////////////////////

void VertexShader::Activate()
{
	assert(pImpl != null);
	pImpl->Activate();
}

//////////////////////////////////////////////////////////////////////

void VertexShader::UpdateConstants(uint index, void const *data)
{
	assert(pImpl != null);
	pImpl->UpdateConstants(index, data);
}

//////////////////////////////////////////////////////////////////////

int VertexShader::GetConstantBufferIndex(char const *name)
{
	assert(pImpl != null);
	return pImpl->GetConstantBufferIndex(name);
}

//////////////////////////////////////////////////////////////////////

