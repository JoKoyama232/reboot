
#include "shadow.hlsli"

cbuffer WorldBuffer		: register( b0 ) { matrix world;	}
cbuffer ShadowBuffer	: register( b1 ) { SHADOW Shadow; }

VS_TO_PS main( VS_INPUT vsIn )
{
	VS_TO_PS vsOut;

	matrix wvp;
	wvp = mul(world, Shadow.mtxLightView);
	wvp = mul(wvp, Shadow.mtxLightProj);
	vsOut.pos	= mul(vsIn.pos, wvp);

	vsOut.nor	= normalize(mul(float4(vsIn.nor.xyz, 0.0f), world));

	vsOut.uv	= vsIn.uv;

	vsOut.wPos	= mul(vsIn.pos, world);

	vsOut.col	= vsIn.col;

	return vsOut;
}