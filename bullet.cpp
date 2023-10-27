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
		g_Bullet[b].spd = 2.0f;
		g_Bullet[b].use = false;


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
		if (!g_Bullet[b].use)continue;
		//PLAYER* player = GetPlayer();
		//XMFLOAT3 p_pos = player->object.GetPositionFloat();
		XMFLOAT3 b_pos = g_Bullet[b].object.GetPositionFloat();
		XMFLOAT3 b_rot = g_Bullet[b].object.GetRotationFloat();

		// 弾の移動処理
		b_pos.x += sinf(b_rot.y) * g_Bullet[b].spd;
		b_pos.z += cosf(b_rot.y) * g_Bullet[b].spd;

		g_Bullet[b].object.SetPosition(XMFLOAT3{ b_pos.x,0.0f,b_pos.z });
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
		if (!g_Bullet[b].use) continue;

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

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Bullet[b].object.GetWorldMatrixPointer(), mtxWorld);

		// 縁取りの設定
		SetFuchi(1);

		// モデル描画
		DrawModel(&g_Bullet[b].object.modelInfo);

		// 縁取りの設定を元に戻す
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
	// 使用されていないバレットを模索
	for (int nCntBullet = g_bulletIdx; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// バレットが使用中か？
		if (g_Bullet[nCntBullet].use || g_bulletIdx == MAX_BULLET)

		{ 
			// 最初に発射されたバレットはまだ使われているか？
			if (!g_Bullet[0].use) {
				// そうであればループさせる
				nCntBullet = 0;
			}
			// 次のインデックスを試す
			continue; 
		}

		// バレットの発射時変数設定
		g_Bullet[nCntBullet].object.SetPosition(pos);
		g_Bullet[nCntBullet].object.SetRotation(rot);
		g_Bullet[nCntBullet].object.SetScale({XMFLOAT3(1.0f,1.0f,1.0f)});
		g_Bullet[nCntBullet].use = true;

		// 最後に発射されたバレットインデックスを更新
		g_bulletIdx = nCntBullet;

		// バレット音
		//PlaySound(SOUND_LABEL_SE_shot000);

		// 発射成功時にバレットのインデックスをを返す（無視してもよい）	
		return g_bulletIdx;

	}

	// 発射不発の場合-1を返す（無視してもよい）
	return -1;
}