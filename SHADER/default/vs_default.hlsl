
#include "default.hlsli"

cbuffer WorldBuffer		: register( b0 ) { matrix world;	}
cbuffer ViewBuffer		: register( b1 ) { matrix view;		}
cbuffer ProjBuffer		: register( b2 ) { matrix proj;		}

VS_TO_PS main( VS_INPUT vsIn )
{
	VS_TO_PS vsOut;

	matrix wvp;
	wvp = mul(world, view);
	wvp = mul(wvp, proj);
	vsOut.pos	= mul(vsIn.pos, wvp);

	vsOut.nor	= normalize(mul(float4(vsIn.nor.xyz, 0.0f), world));

	vsOut.uv	= vsIn.uv;

	vsOut.wPos	= mul(vsIn.pos, world);

	vsOut.col	= vsIn.col;

	return vsOut;
}