// デブリ処理 [debris.cpp]
// Author 小井詰　礼
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "satellite.h"
#include "sprite.h"
#include <time.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_SATELLITE		"Data/model/satellite.obj"			// 読み込むモデル名

#define	VALUE_MOVE			(2.0f)							// 移動量

#define TEXTURE_MAX			(1)								// テクスチャの数


//*****************************************************************************
// グローバル変数
//*****************************************************************************


static float        g_LastUpdate;
SATELLITE g_Satellite[MAX_SATELLITE];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitSatellite(void) {

	for (int i = 0; i < MAX_SATELLITE; i++)
	{
		LoadModel(MODEL_SATELLITE, &g_Satellite[i].object.modelInfo);
		GetModelDiffuse(&g_Satellite[i].object.modelInfo, g_Satellite[i].object.modelDiffuse);
		g_Satellite[i].object.load = true;
		g_Satellite[i].object.SetPosition(XMFLOAT3{ rand() % 1000 + 0.0f, rand() % 1000 - 500.0f, rand() % 1000 + 0.0f });
		g_Satellite[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Satellite[i].object.SetScale(XMFLOAT3{ 0.5f, 0.5f, 0.5f });
		g_Satellite[i].time = 0.0f;
		g_Satellite[i].speed = 0.0f;			// 移動スピードクリア
		g_Satellite[i].use = true;
		g_Satellite[i].flag_rotate = true;
		g_Satellite[i].object.draw = true;
		g_Satellite[i].size = SATELLITE_SIZE;
		g_LastUpdate = 0.0f;

	}
	return S_OK;

}

void UninitSatellite(void) {
	// モデルの解放処理
	for (int i = 0; i < MAX_SATELLITE; i++)
	{

		if (g_Satellite[i].object.load)
		{
			UnloadModel(&g_Satellite[i].object.modelInfo);
			g_Satellite[i].object.load = false;
		}
	}
}

void UpdateSatellite(void) {
	// デブリ変数取得
	for (int i = 0; i < MAX_SATELLITE; i++)
	{
		// デブリの使用フラグを確認
		if (!g_Satellite[i].use)
		{
			g_Satellite[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_Satellite[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Satellite[i].object.GetRotationFloat();

		if (!g_Satellite[i].flag_rotate)continue;
		{
			// ぐるぐる回転
			rotation.x += (i + 1) * 0.005f;
			rotation.y += ((rand() % 5) + 1) * 0.001f;
			rotation.z += ((MAX_SATELLITE + 1) - i) * 0.005f;
		}

		// 移動回転を反映
		g_Satellite[i].object.SetPosition(position);
		g_Satellite[i].object.SetRotation(rotation);
	}

}

void DrawSatellite(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_SATELLITE; i++)
	{
		if (!g_Satellite[i].object.draw) continue;
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		XMFLOAT3 scale = g_Satellite[i].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		XMFLOAT3 rotation = g_Satellite[i].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		XMFLOAT3 position = g_Satellite[i].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Satellite[i].object.GetWorldMatrixPointer(), mtxWorld);

		// モデル描画
		DrawModel(&g_Satellite[i].object.modelInfo);

		SetFuchi(0);

		// カリング設定を戻す
		SetCullingMode(CULL_MODE_BACK);

	}
}

SATELLITE* GetSatellite(void) {

	return &g_Satellite[0];
}

