
#include "extra.hlsli"

cbuffer MaterialBuffer	: register( b0 ) { Material		mat;		}
cbuffer LightBuffer		: register( b1 ) { Light		light;		}
cbuffer FogBuffer		: register( b2 ) { Fog			fog;		}
cbuffer Fuchi			: register( b3 ) { Outline		outline;	}
cbuffer CameraBuffer	: register( b4 ) { float4		cam;		}

Texture2D		g_Texture		: register(t0);
SamplerState	g_SamplerState	: register(s0);

float4 ColorMaterial	( VS_TO_PS psIn );
float4 ColorLight		( VS_TO_PS psIn, float4 xcol );
float4 ColorFog			( VS_TO_PS psIn, float4 xcol );
float4 ColorOutline		( VS_TO_PS psIn, float4 xcol );

float4 LightDirectional	( VS_TO_PS psIn, float4 xcol, int i );
float4 LightPoint		( VS_TO_PS psIn, float4 xcol, int i );

PS_OUT main( VS_TO_PS psIn)
{
	PS_OUT psOut;

	float4 col = 0.f;

	{
		col = ColorMaterial	(psIn);
		col = ColorLight	(psIn, col);
		col = ColorFog		(psIn, col);
		col = ColorOutline	(psIn, col);
	}

	psOut.col = col;

	return psOut;
}

float4 ColorMaterial(VS_TO_PS psIn)
{
	float4 col = 0.f;

	if (!mat.noTexSampling)
	{// �}�e���A���g�p��

		col = g_Texture.Sample(g_SamplerState, psIn.uv) * psIn.col;
	}
	else
	{// �}�e���A�����g�p��

		col = psIn.col;
	}

	return col;
}

float4 ColorLight(VS_TO_PS psIn, float4 xcol)
{
	// ���C�g���g�p��
	if (!light.enable) return xcol * mat.diffuse;

	// �v�Z�p�ϐ�
	float4 col, tmpCol;
	col = tmpCol = 0.f;

	for (int i = 0; i < 5; ++i)
	{// ���C�g�ő吔�����[�v

		// ���C�g�I�t���͖���
		if (!light.flags[i].y) continue;

		switch (light.flags[i].x)
		{
		//-------------------------------- �f�B���N�V���i�����C�g
		case 1:
			tmpCol = LightDirectional(psIn, xcol, i);
			break;

		//-------------------------------- �|�C���g���C�g
		case 2:
			tmpCol = LightPoint(psIn, xcol, i);
			break;

		//-------------------------------- ���C�g�Ȃ�
		default:
			tmpCol = 0.f;
			break;
		}

		col += tmpCol;
	}

	// �o�͒l�ɑ��
	col.a = psIn.col.a * mat.diffuse.a;

	return col;
}

float4 ColorFog(VS_TO_PS psIn, float4 xcol)
{
	// �t�H�O���g�p��
	if (!fog.enable) return xcol;

	// �[�x�l�擾
	float z = psIn.pos.z * psIn.pos.w;

	float f = saturate((fog.distance.y - z) / (fog.distance.y - fog.distance.x));

	return (f * xcol.rgb + (1.f - f) * fog.fogColor.rgb, xcol.a);
}

float4 ColorOutline(VS_TO_PS psIn, float4 xcol)
{
	// �A�E�g���C�����g�p��
	if (!outline.enable) return xcol;

	float4 col = 0.f;

	// �J��������̃x�N�g���Ɩ@���̓��ς��Z�o
	float angle = dot(normalize(psIn.wPos.xyz - cam.xyz), normalize(psIn.nor.xyz));

	if (angle > -0.3f)
	{// �A�E�g���C���̏ꍇ

		col.rb = 1.f;
		col.g  = 0.f;
	}
	else
	{
		col = xcol;
	}

	return col;
}

float4 LightDirectional(VS_TO_PS psIn, float4 xcol, int i)
{
	// ���C�g�̕����̋t�x�N�g�����K��
	float3 lightDir = -normalize(light.direction[i].xyz);

	// �����o�[�g
	float lightNor = dot(lightDir, psIn.nor.xyz);

	// �n�[�t�����o�[�g
	lightNor = 0.5f + lightNor * 0.5f;

	// �ŏI���C�g���ʂ̌v�Z
	return xcol * mat.diffuse * lightNor * light.diffuse[i];
}

float4 LightPoint(VS_TO_PS psIn, float4 xcol, int i)
{
	// ��������e���[���h���W�ւ̋t�x�N�g�����K��
	float3 worldDir = -normalize(psIn.wPos.xyz - light.position[i].xyz);

	// �����o�[�g
	float lightNor = dot(worldDir, psIn.nor.xyz);

	// ���C�g�̏����Ȍ��ʂ��v�Z
	float4 col = xcol * mat.diffuse * lightNor * light.diffuse[i];

	// ��������e���[���h���W�ւ̋������Z�o
	float distance = length(psIn.wPos - light.position[i]);

	// ���������𔽉f
	col *= saturate((light.attenuation[i].x - distance) / light.attenuation[i].x);

	return col;
}
