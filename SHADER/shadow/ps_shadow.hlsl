
#include "shadow.hlsli"

cbuffer MaterialBuffer	: register( b0 ) { Material		mat;		}
cbuffer LightBuffer		: register( b1 ) { Light		light;		}
cbuffer FogBuffer		: register( b2 ) { Fog			fog;		}
cbuffer Fuchi			: register( b3 ) { Outline		outline;	}
cbuffer CameraBuffer	: register( b4 ) { float4		cam;		}
cbuffer ShadowBuffer	: register( b5 ) { SHADOW Shadow; }

Texture2D		g_Texture				: register(t0);
Texture2D		g_ShadowMapTexture		: register(t1);		// 深度値をテクスチャにしたもの（いわゆるシャドウマップ）

SamplerState	g_SamplerState			: register(s0);
SamplerState	g_ShadowSamplerState	: register(s1);		// シャドウマップをサンプリングするためのサンプラー

float4 ColorMaterial	( VS_TO_PS psIn );
float4 ColorLight		( VS_TO_PS psIn, float4 xcol );
float4 ColorFog			( VS_TO_PS psIn, float4 xcol );
float4 ColorOutline		( VS_TO_PS psIn, float4 xcol );
float4 ColorShadow		( VS_TO_PS psIn, float4 xcol );

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
		col = ColorShadow	(psIn, col);
	}

	psOut.col = col;

	return psOut;
}

float4 ColorMaterial(VS_TO_PS psIn)
{
	float4 col = 0.f;

	if (!mat.noTexSampling)
	{// マテリアル使用時

		col = g_Texture.Sample(g_SamplerState, psIn.uv) * psIn.col;
	}
	else
	{// マテリアル未使用時

		col = psIn.col;
	}

	return col;
}

float4 ColorLight(VS_TO_PS psIn, float4 xcol)
{
	// ライト未使用時
	if (!light.enable) return xcol * mat.diffuse;

	// 計算用変数
	float4 col, tmpCol;
	col = tmpCol = 0.f;

	for (int i = 0; i < 5; ++i)
	{// ライト最大数分ループ

		// ライトオフ時は無視
		if (!light.flags[i].y) continue;

		switch (light.flags[i].x)
		{
		//-------------------------------- ディレクショナルライト
		case 1:
			tmpCol = LightDirectional(psIn, xcol, i);
			break;

		//-------------------------------- ポイントライト
		case 2:
			tmpCol = LightPoint(psIn, xcol, i);
			break;

		//-------------------------------- ライトなし
		default:
			tmpCol = 0.f;
			break;
		}

		col += tmpCol;
	}

	// 出力値に代入
	col.a = psIn.col.a * mat.diffuse.a;

	return col;
}

float4 ColorFog(VS_TO_PS psIn, float4 xcol)
{
	// フォグ未使用時
	if (!fog.enable) return xcol;

	// 深度値取得
	float z = psIn.pos.z * psIn.pos.w;

	float f = saturate((fog.distance.y - z) / (fog.distance.y - fog.distance.x));

	return (f * xcol.rgb + (1.f - f) * fog.fogColor.rgb, xcol.a);
}

float4 ColorOutline(VS_TO_PS psIn, float4 xcol)
{
	// アウトライン未使用時
	if (!outline.enable) return xcol;

	float4 col = 0.f;

	// カメラからのベクトルと法線の内積を算出
	float angle = dot(normalize(psIn.wPos.xyz - cam.xyz), normalize(psIn.nor.xyz));

	if (angle > -0.3f)
	{// アウトラインの場合

		col.rb = 1.f;
		col.g  = 0.f;
	}
	else
	{
		col = xcol;
	}

	return col;
}

float4 ColorShadow(VS_TO_PS psIn, float4 xcol)
{
	float shadowColor = 1.0f;

	// テクセルを計算
	float2 texel;
	texel.x = psIn.lightPos.x / psIn.lightPos.w * 0.5f + 0.5f;
	texel.y = psIn.lightPos.y / psIn.lightPos.w * -0.5f + 0.5f;

	float depth = psIn.lightPos.z / psIn.lightPos.w - 0.00005f;
	float sm = g_ShadowMapTexture.Sample(g_ShadowSamplerState, texel).x;

	// 現在の深度値とシャドウマップ上の深度値を比較
	if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1)
	{
		if (sm > depth)
		{// 非影時の彩度

			shadowColor = 1.0f;
		}
		else
		{// 影時の彩度

			shadowColor = 0.4f;
		}
	}

	xcol.rgb *= shadowColor;

	//影を足す
	return xcol;
}

float4 LightDirectional(VS_TO_PS psIn, float4 xcol, int i)
{
	// ライトの方向の逆ベクトル正規化
	float3 lightDir = -normalize(light.direction[i].xyz);

	// ランバート
	float lightNor = dot(lightDir, psIn.nor.xyz);

	// ハーフランバート
	lightNor = 0.5f + lightNor * 0.5f;

	// 最終ライト効果の計算
	return xcol * mat.diffuse * lightNor * light.diffuse[i];
}

float4 LightPoint(VS_TO_PS psIn, float4 xcol, int i)
{
	// 光源から各ワールド座標への逆ベクトル正規化
	float3 worldDir = -normalize(psIn.wPos.xyz - light.position[i].xyz);

	// ランバート
	float lightNor = dot(worldDir, psIn.nor.xyz);

	// ライトの純粋な効果を計算
	float4 col = xcol * mat.diffuse * lightNor * light.diffuse[i];

	// 光源から各ワールド座標への距離を算出
	float distance = length(psIn.wPos - light.position[i]);

	// 距離減衰を反映
	col *= saturate((light.attenuation[i].x - distance) / light.attenuation[i].x);

	return col;
}
