
#include "default.hlsli"

Texture2D		g_Texture		: register(t0);
SamplerState	g_SamplerState	: register(s0);

PS_OUT main( VS_TO_PS psIn)
{
	PS_OUT psOut;

	// �s�N�Z���F�v�Z
	psOut.col = g_Texture.Sample(g_SamplerState, psIn.uv) * psIn.col;

	return psOut;
}