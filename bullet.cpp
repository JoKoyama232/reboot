// 弾丸処理 [Player.cpp]
// Author 
//=============================================================================
#include "model.h"
#include "main.h"

#include "Player.h"
#include "bullet.h"
#include "input.h"
#include "debugproc.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BULLET		"Data/model/cone.obj"			// 読み込むモデル名
															// 今はモチがないので仮

#define	VALUE_MOVE			(3.0f)							// 移動量

static float		roty = 0.0f;

//*****************************************************************************
// グローバル変数
//*****************************************************************************

BULLET g_Bullet[MAX_BULLET];


//初期化処理
HRESULT InitBullet(void)
{
	for (int b = 0; b < MAX_BULLET; b++)
	{
		LoadModel(MODEL_BULLET, &g_Bullet[b].object.modelInfo);
		GetModelDiffuse(&g_Bullet[b].object.modelInfo, g_Bullet[b].object.modelDiffuse);
		g_Bullet[b].object.load = true;

		g_Bullet[b].object.SetPosition(XMFLOAT3{ 0.0f,0.0f,0.0f });
		g_Bullet[b].object.SetRotation(XMFLOAT3{ 0.0f,0.0f,0.0f });
		g_Bullet[b].object.SetScale(XMFLOAT3{ 0.5f,0.5f,0.5f });
		g_Bullet[b].time = 0.0f;
		g_Bullet[b].spd = 0.0f;
		g_Bullet[b].use = true;


	}
	return S_OK;
}

//終了処理
void UninitBullet(void)
{
	//モデルの解放処理
	for (int b = 0; b < MAX_BULLET; b++)
	{
		if (g_Bullet[b].object.load)
		{
			UnloadModel(&g_Bullet[b].object.modelInfo);
			g_Bullet[b].object.load = false;
		}
	}
}

//更新処理
void UpdateBullet(void)
{

	for (int b = 0; b < MAX_BULLET; b++)
	{
		PLAYER* player = GetPlayer();
		XMFLOAT3 p_pos = player->object.GetPositionFloat();
		XMFLOAT3 b_pos = g_Bullet[b].object.GetPositionFloat();

		// 弾の移動処理
		g_Bullet[b].spd = VALUE_MOVE;
		roty = XM_PI;


		if (b_pos.x > p_pos.x + 20.0f
			|| b_pos.x < p_pos.x - 20.0f
			|| b_pos.z > p_pos.z + 20.0f
			|| b_pos.z < p_pos.z - 20.0f)
		{
			g_Bullet[b].use = false;
		}
	}

}

//描画処理
void DrawBullet(void)
{

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	for (int b = 0; b < MAX_BULLET; b++)
	{
		// スケールを反映
		XMFLOAT3 scale = g_Bullet[b].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		XMFLOAT3 rotation = g_Bullet[b].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		XMFLOAT3 position = g_Bullet[b].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);


		XMStoreFloat4x4(g_Bullet[b].object.GetWorldMatrixPointer(), mtxWorld);


		// 縁取りの設定
		SetFuchi(1);

		// モデル描画
		DrawModel(&g_Bullet[b].object.modelInfo);

		SetFuchi(0);

		// カリング設定を戻す
		SetCullingMode(CULL_MODE_BACK);
	}
}

BULLET* GetBullet(void) 
{
	return &(g_Bullet[0]);
}

//=============================================================================
// 弾のパラメータをセット
//=============================================================================
int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot)
{
	int nIdxBullet = -1;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		XMFLOAT3 b_pos = g_Bullet[nCntBullet].object.GetPositionFloat();
		XMFLOAT3 b_rot = g_Bullet[nCntBullet].object.GetRotationFloat();
		XMFLOAT3 b_scl = g_Bullet[nCntBullet].object.GetScaleFloat();


		if (!g_Bullet[nCntBullet].use)
		{
			b_pos = pos;
			b_rot = rot;
			b_scl = { 1.0f, 1.0f, 1.0f };
			g_Bullet[nCntBullet].use = true;

			nIdxBullet = nCntBullet;

			//PlaySound(SOUND_LABEL_SE_shot000);

			break;
		}
	}
	return nIdxBullet;
}