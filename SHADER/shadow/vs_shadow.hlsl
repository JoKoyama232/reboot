
#include "shadow.hlsli"

cbuffer WorldBuffer		: register( b0 ) { matrix world;	}
cbuffer ViewBuffer		: register( b1 ) { matrix view;		}
cbuffer ProjBuffer		: register( b2 ) { matrix proj;		}
cbuffer ShadowBuffer	: register( b3 ) { SHADOW Shadow; }

VS_TO_PS main( VS_INPUT vsIn )
{
	VS_TO_PS vsOut;

	float4 pos = float4(vsIn.pos.xyz, 1.0f);
	float4 modelPos = mul(pos, world);
	pos = mul(modelPos, view);
	pos = mul(pos, proj);
	vsOut.pos = pos;

	vsOut.nor	= normalize(mul(float4(vsIn.nor.xyz, 0.0f), world));

	vsOut.uv	= vsIn.uv;

	vsOut.wPos	= mul(vsIn.pos, world);

	vsOut.col	= vsIn.col;

	float4 lightSpacePos = mul(modelPos, Shadow.mtxLightView);
	lightSpacePos = mul(lightSpacePos, Shadow.mtxLightProj);
	vsOut.lightPos = lightSpacePos;

	return vsOut;
}