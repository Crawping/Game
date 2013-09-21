//////////////////////////////////////////////////////////////////////

cbuffer TransformMatrix : register(b0)
{
	matrix modelViewProjection;
};

//////////////////////////////////////////////////////////////////////

struct VS_Input
{
	float3 pos		: POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR;
};

//////////////////////////////////////////////////////////////////////

struct VS_Output
{
	float4	pos		: SV_POSITION;
	float2	uv		: TEXCOORD0;
	float4	color	: COLOR;
};

//////////////////////////////////////////////////////////////////////

VS_Output main(VS_Input i)
{
	VS_Output o;
	o.pos = mul(modelViewProjection, float4(i.pos.x, i.pos.y, i.pos.z, 1.0f));
	o.uv = i.uv;
	o.color = i.color;
	return o;
}
