
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
	float4 lightPos	: POSITION1;
};

struct PS_OUT
{
	float4 col		: SV_Target;
};

struct Material
{
	float4		ambient;
	float4		diffuse;
	float4		specular;
	float4		emission;
	float		shininess;
	int			noTexSampling;
	float		padding[2];
};

struct Light
{
	float4		direction[5];
	float4		position[5];
	float4		diffuse[5];
	float4		ambient[5];
	float4		attenuation[5];
	int4		flags[5];
	int			enable;
	int			padding[3];
};

struct Fog
{
	float4		distance;
	float4		fogColor;
	int			enable;
	float		padding[3];
};

struct Outline
{
	int			enable;
	int			padding[3];
};

struct SHADOW
{
	float4x4	mtxLightView;
	float4x4	mtxLightProj;
};