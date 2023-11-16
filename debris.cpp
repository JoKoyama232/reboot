// デブリ処理 [debris.cpp]
// Author 小井詰　礼
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "debris.h"
#include "sprite.h"
#include <time.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_DEBRIS		"Data/model/debris.obj"			// 読み込むモデル名

#define	VALUE_MOVE			(2.0f)							// 移動量

#define TEXTURE_MAX			(1)								// テクスチャの数


//*****************************************************************************
// グローバル変数
//*****************************************************************************


static float        g_LastUpdate;
DEBRIS g_Debris[MAX_DEBRIS];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitDebris(void) {

	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		LoadModel(MODEL_DEBRIS, &g_Debris[i].object.modelInfo);
		GetModelDiffuse(&g_Debris[i].object.modelInfo, g_Debris[i].object.modelDiffuse);
		g_Debris[i].object.load = true;

		g_Debris[i].object.SetPosition(XMFLOAT3{ 0.0f+i*100.0f, 0.0f, 100.0f });
		g_Debris[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Debris[i].object.SetScale(XMFLOAT3{ 0.1f, 0.1f, 0.1f });
		g_Debris[i].str = 100.0f;
		g_Debris[i].str_max = 100.0f;
		g_Debris[i].time = 0.0f;
		g_Debris[i].speed = 0.0f;			// 移動スピードクリア
		g_Debris[i].use = true;
		g_Debris[i].size = DEBRIS_SIZE;
		g_LastUpdate = 0.0f;

	}
		return S_OK;
	
}

void UninitDebris(void) {
	// モデルの解放処理
	for (int i = 0; i < MAX_DEBRIS; i++)
	{

		if (g_Debris[i].object.load)
		{
			UnloadModel(&g_Debris[i].object.modelInfo);
			g_Debris[i].object.load = false;
		}
	}
}

void UpdateDebris(void) {
	// デブリ変数取得
	for (int i = 0; i < MAX_DEBRIS; i++)
	{

		XMFLOAT3 position = g_Debris[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Debris[i].object.GetRotationFloat();

		if (g_Debris[i].use)
		{
		}

		// 移動回転を反映
		g_Debris[i].object.SetPosition(position);
		g_Debris[i].object.SetRotation(rotation);
	}

}

void DrawDebris(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		if (!g_Debris[i].use) continue;
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

	// スケールを反映
	XMFLOAT3 scale = g_Debris[i].object.GetScaleFloat();
	mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	XMFLOAT3 rotation = g_Debris[i].object.GetRotationFloat();
	mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	XMFLOAT3 position = g_Debris[i].object.GetPositionFloat();
	mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(g_Debris[i].object.GetWorldMatrixPointer(), mtxWorld);

	// 縁取りの設定
	SetFuchi(1);

	// モデル描画
	DrawModel(&g_Debris[i].object.modelInfo);

	SetFuchi(0);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);

	}
}

DEBRIS* GetDebris(void) {
	
	return &g_Debris[0];
}

