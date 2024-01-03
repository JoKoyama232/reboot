// デブリ処理 [debris.cpp]
// Author 岩田
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "title_debris.h"
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
TDEBRIS g_Debris[MAX_DEBRIS];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitTitleDebris(void) {

	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		LoadModel(MODEL_DEBRIS, &g_Debris[i].object.modelInfo);
		GetModelDiffuse(&g_Debris[i].object.modelInfo, g_Debris[i].object.modelDiffuse);
		g_Debris[i].object.load = true;
		g_Debris[i].object.SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Debris[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Debris[i].object.SetScale(XMFLOAT3{ 0.1f, 0.1f, 0.1f });
		g_Debris[i].time = 0.0f;
		g_Debris[i].speed = 0.0f;			// 移動スピードクリア
		g_Debris[i].use = true;
		g_Debris[i].flag_rotate = true;
		g_Debris[i].object.draw = true;
		g_Debris[i].size = DEBRIS_SIZE;
		g_LastUpdate = 0.0f;


		//タイトルだけ位置の指定
		g_Debris[0].object.SetPosition(XMFLOAT3{ -100.0f,   0.0f, 1000.0f });
		g_Debris[1].object.SetPosition(XMFLOAT3{  100.0f, -50.0f,  300.0f });
		g_Debris[2].object.SetPosition(XMFLOAT3{ -200.0f,  20.0f,  200.0f });
		g_Debris[3].object.SetPosition(XMFLOAT3{ -150.0f,-200.0f,  300.0f });
		g_Debris[4].object.SetPosition(XMFLOAT3{  150.0f,-100.0f,  100.0f });
		g_Debris[5].object.SetPosition(XMFLOAT3{  150.0f,  10.0f,  100.0f });
	}
	return S_OK;

}

void UninitTitleDebris(void) {
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

void UpdateTitleDebris(void) {
	// デブリ変数取得
	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		// デブリの使用フラグを確認
		if (!g_Debris[i].use)
		{
			g_Debris[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_Debris[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Debris[i].object.GetRotationFloat();

		if (!g_Debris[i].flag_rotate)continue;
		{
			// ぐるぐる回転
			rotation.x += (i + 1) * 0.001f;
			rotation.y += ((rand() % 5) + 1) * 0.001f;
			rotation.z += ((MAX_DEBRIS + 1) - i) * 0.001f;
		}

		// 移動回転を反映
		g_Debris[i].object.SetPosition(position);
		g_Debris[i].object.SetRotation(rotation);
	}

}

void DrawTitleDebris(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		if (!g_Debris[i].object.draw) continue;
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

		// モデル描画
		DrawModel(&g_Debris[i].object.modelInfo);

		SetFuchi(0);

		// カリング設定を戻す
		SetCullingMode(CULL_MODE_BACK);

	}
}

TDEBRIS* GetTitleDebris(void) {

	return &g_Debris[0];
}

