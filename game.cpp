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
#include "score.h"

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
static RECT	g_windowPos;


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

	InitScore();

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

	UninitScore();

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

		DrawScore();

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
	PLAYER* player = GetPlayer();			// プレイヤー情報
	DEBRIS* debris = GetDebris();			// デブリ情報
	BULLET* bullet = GetBullet();			// 弾丸情報
	BASE* base = GetBase();					// 基地情報
	p_pos = GetPlayer()->object.GetPositionFloat();
	basepos = GetBase()->object.GetPositionFloat();

	// プレイヤーが使われていないのなら当たり判定も必要ないためまずプレイヤーの使用フラグ確認
	if (!player->use) return;

	// 弾丸毎の当たり判定----------------------------------------------------
	for (int b = 0; b < MAX_BULLET; b++)
	{
		// 弾丸の使用フラグを確認、またペアレントされていれば当たり判定を行われない
		if (!bullet[b].use || bullet[b].object.GetParent()) continue;

		// バレット対デブリ当たり判定
		for (int d = 0; d < MAX_DEBRIS; d++)
		{
			// デブリの使用フラグ確認
			if (!debris[d].use) continue;
			
			// 弾丸とデブリの座標をゲット
			b_pos = bullet[b].object.GetPositionFloat();
			d_pos = debris[d].object.GetPositionFloat();

			// 弾丸とデブリの当たり判定
			if (!CollisionBC(b_pos, d_pos, bullet[b].size, debris[d].size)) continue;
			
			// 弾丸とデブリの当たり反応（真）
			bullet[b].spd = 0.0f;
			bullet[b].object.SetParent(&debris[d].object);
		}
	}
	//----------------------------------------------------------------------

	// プレイヤーの当たり判定-------------------------------------------------

	bool unload = false;		// 保持しているデブリを回収
	// プレイヤーと基地の当たり判定
	if (CollisionBC(p_pos, basepos, player->size, base->size))
	{
		// プレイヤーと基地の当たり反応（真）
	}

	// デブリ毎に判定
	for (int d = 0; d < MAX_DEBRIS; d++)
	{
		// デブリの使用フラグ確認
		if (!debris[d].use) continue;

		if (!CollisionBC(p_pos, d_pos, player->size, debris[d].size)) continue;
		
		// プレイヤーとデブリの当たり反応（真）
		PlaySound(SOUND_LABEL_SE_ABSORB);
		debris[d].use = false;
		
		//エフェクトのイメージは吸い込まれる感じ(マイクラの経験値が近い)
	}
	//----------------------------------------------------------------------

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
