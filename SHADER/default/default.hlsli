
struct VS_INPUT
{
	float4 pos		: POSITION0;
	float4 nor		: NORMAL0;
	float4 col		: COLOR0;
	float2 uv		: TEXCOORD0;
};

struct VS_TO_PS
{
	float4 pos		: SV_POSITION;
	float4 nor		: NORMAL0;
	float4 col		: COLOR0;
	float2 uv		: TEXCOORD0;
	float4 wPos		: POSITION0;
};

struct PS_OUT
{
	float4 col		: SV_Target;
};