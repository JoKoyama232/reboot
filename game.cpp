//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"

#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "WorldSpaceClass.h"
#include "Skybox.h"
#include "clock.h"
#include "Player.h"
#include "debris.h"
#include "bullet.h"
#include "base.h"

#include "game.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// グローバル変数
//*****************************************************************************

static bool	g_bPause = true;	// ポーズON/OFF
static RECT				g_windowPos;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	InitPlayer();

	InitDebris();

	InitBase();

	//弾(モチ)の初期化
	InitBullet();

	//時計の初期化
	InitClock();

	InitSkybox();

	//マウス非表示
	ShowCursor(false);



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	UninitPlayer();

	UninitDebris();

	UninitBase();

	UninitSkybox();

	//弾(モチ)の終了処理
	UninitBullet();

	// 時計の終了処理
	UninitClock();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	UpdatePlayer();

	UpdateDebris();

	UpdateBase();
	//弾(モチ)の更新処理
	UpdateBullet();

	// 時計の更新処理
	UpdateClock();

	UpdateSkybox();

	//当たり判定
	CheckHit();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	XMFLOAT3 pos;

	// プレイヤー視点
	pos = GetPlayer()->object.GetPositionFloat();
	pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
	SetCameraAT(pos);
	SetCamera();

	DrawSkybox();

	DrawPlayer();

	DrawDebris();

	DrawBase();
	//弾(モチ)の描画処理
	DrawBullet();

	// 2Dの物を描画する処理
	{
		// Z比較なし
		SetDepthEnable(false);

		// ライティングを無効
		SetLightEnable(false);

		//ここから下に
		DrawClock();

		DrawPlayerUI();

		DrawPlayerRestBullet();
		// ライティングを有効に
		SetLightEnable(true);

		// Z比較あり
		SetDepthEnable(true);
	}

}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	XMFLOAT3 p_pos, d_pos, b_pos, basepos;
	PLAYER* player = GetPlayer();
	DEBRIS* debris = GetDebris();
	BULLET* bullet = GetBullet();
	BASE* base = GetBase();
	p_pos = GetPlayer()->object.GetPositionFloat();
	basepos = GetBase()->object.GetPositionFloat();
	for (int b = 0; b < MAX_BULLET; b++)
	{
		for (int d = 0; d < MAX_DEBRIS; d++)
		{
			//弾丸とデブリの座標をゲット
			b_pos = bullet[b].object.GetPositionFloat();
			d_pos = debris[d].object.GetPositionFloat();

			//プレイヤーとデブリ
			if ((player->use == false) || (debris[d].use == false))
				continue;
			if (CollisionBC(p_pos, d_pos, player->size, debris[d].size))
			{
				if (!bullet[b].object.GetParent() == NULL)
				{
					PlaySound(SOUND_LABEL_SE_ABSORB);
					bullet[b].use = false;
					bullet[b].spd = 1.0f;
					debris[d].use = false;
					bullet[b].object.SetParent(NULL);
				}
				//エフェクトのイメージは吸い込まれる感じ(マイクラの経験値が近い)
			}

			//モチとデブリ
			if ((bullet[b].use == false) || (debris[d].use == false))
				continue;
			if (CollisionBC(b_pos, d_pos, bullet[b].size, debris[d].size))
			{
				if (bullet[b].object.GetParent() == NULL)
				{
					bullet[b].spd = 0.0f;
					bullet[b].object.SetParent(&debris[d].object);
				}
			}

			//拠点とプレイヤー
			if ((player->use == false) || (base->use == false))
				continue;
			if (CollisionBC(p_pos, basepos, player->size, base->size))
			{
				if (bullet[b].object.GetParent() == NULL)
				{
					bullet[b].use = false;
				}
			}

		}
	}
}




//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらTRUE
//=============================================================================
BOOL CollisionBC(XMFLOAT3 pos1, XMFLOAT3 pos2, float r1, float r2)
{
	BOOL ans = FALSE;						// 外れをセットしておく

	float len = (r1 + r2) * (r1 + r2);		// 半径を2乗した物
	XMVECTOR temp = XMLoadFloat3(&pos1) - XMLoadFloat3(&pos2);
	temp = XMVector3LengthSq(temp);			// 2点間の距離（2乗した物）
	float lenSq = 0.0f;						// 型変換用float型変数の宣言と初期化
	XMStoreFloat(&lenSq, temp);				// 比較処理のためにfloat型に型変換

	// 半径を2乗した物より距離が短い？
	if (len > lenSq)
	{
		ans = TRUE;	// 当たっている
	}

	return ans;
}
