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
#define	MODEL_BULLET		"Data/model/ball.obj"			// 読み込むモデル名
															// 今はモチがないので仮
static int		    g_bulletIdx = 0;						// 最後に発射された弾のインデックス

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
		g_Bullet[b].object.SetScale(XMFLOAT3{ 1.0f,1.0f,1.0f });
		g_Bullet[b].time = 0.0f;
		g_Bullet[b].spd = 5.0f;
		g_Bullet[b].Halpha = 1.0f;
		g_Bullet[b].use = true;
		g_Bullet[b].flag_Halpha = false;
		g_Bullet[b].object.draw = false;
		g_Bullet[b].size = BULLET_SIZE;


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
		// 弾丸にペアレントがありかつそのペアレントの描画フラグが偽の時
		if (g_Bullet[b].object.GetParent() && !g_Bullet[b].object.GetParent()->draw) {
			g_Bullet[b].use = false;
			
		}

		// 弾丸の使用フラグを確認
		if (!g_Bullet[b].use) 
		{
			g_Bullet[b].object.draw = false;
			continue;
		}
		
		//座標とrotationのゲット
		XMFLOAT3 b_pos = g_Bullet[b].object.GetPositionFloat();
		XMFLOAT3 b_rot = g_Bullet[b].object.GetRotationFloat();

		// 弾の移動処理
		b_pos.x += sinf(b_rot.y) * g_Bullet[b].spd;
		b_pos.y += sinf(b_rot.x) * g_Bullet[b].spd;
		b_pos.z += cosf(b_rot.y) * g_Bullet[b].spd;

		g_Bullet[b].object.SetPosition(b_pos);
		g_Bullet[b].object.SetRotation(b_rot);

	}
}

//描画処理
void DrawBullet(void)
{

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int b = 0; b < MAX_BULLET; b++)
	{
		if (!g_Bullet[b].object.draw) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

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

		if (g_Bullet[b].object.GetParent())
		{
			mtxWorld = XMMatrixMultiply(mtxWorld,XMLoadFloat4x4(g_Bullet[b].object.GetParent()->GetWorldMatrixPointer()));
		}

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Bullet[b].object.GetWorldMatrixPointer(), mtxWorld);

		// モデル描画
		DrawModel(&g_Bullet[b].object.modelInfo);

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

	// 使用されていないバレットを模索
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// バレットが使用中か？
		if (g_Bullet[nCntBullet].object.draw)continue;
		
		// バレットの発射時変数設定
		g_Bullet[nCntBullet].object.SetPosition(pos);
		g_Bullet[nCntBullet].object.SetRotation(rot);
		g_Bullet[nCntBullet].object.SetScale({ XMFLOAT3(1.0f,1.0f,1.0f) });
		g_Bullet[nCntBullet].use = true;
		g_Bullet[nCntBullet].object.draw = true;

		// 最後に発射されたバレットインデックスを更新
		nIdxBullet = nCntBullet;

		break;
	}
	// 発射成功時にバレットのインデックスをを返す（無視してもよい）	
	return nIdxBullet;
}