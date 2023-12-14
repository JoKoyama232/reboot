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
#include "antenna.h"
#include "solarpanel.h"
#include "hatch.h"
#include "pod.h"
#include "satellite.h"
#include "rocket.h"
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
static int flag_score = 0;
static RECT	g_windowPos;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	InitPlayer();

	InitDebris();

	InitAntenna();

	InitPanel();

	InitHatch();

	InitPod();

	InitSatellite();

	InitRocket();

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

	UninitAntenna();

	UninitPanel();

	UninitHatch();

	UninitPod();

	UninitSatellite();

	UninitRocket();

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

	UpdateAntenna();

	UpdatePanel();

	UpdateHatch();

	UpdatePod();

	UpdateSatellite();

	UpdateRocket();

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
	SetCameraAT(pos);
	SetCamera();

	DrawSkybox();

	DrawPlayer();

	DrawDebris();

	DrawAntenna();

	//DrawPanel();

	//DrawHatch();

	//DrawPod();

	//DrawSatellite();

	//DrawRocket();

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
	// 各オブジェクトの位置を定義
	XMFLOAT3 podPos, basePos, hatchPos, panelPos, playerPos,
		debrisPos, bulletPos, rokeckPos, antennaPos, satellitePos;

	// 各オブジェクトの情報を取得
	POD* pod = GetPod();					// ポッド情報
	BASE* base = GetBase();					// 基地情報
	HATCH* hatch = GetHatch();				// ハッチ情報
	PANEL* panel = GetPanel();				// ソーラーパネル情報
	PLAYER* player = GetPlayer();			// プレイヤー情報
	DEBRIS* debris = GetDebris();			// デブリ情報
	BULLET* bullet = GetBullet();			// 弾丸情報
	ROCKET* rocket = GetRocket();			// ロケット情報
	ANTENNA* antenna = GetAntenna();		// アンテナ情報
	SATELLITE* satellite = GetSatellite();  // サテライト情報

	// 1つしか使わないオブジェクトの位置をあらかじめ定義
	playerPos = GetPlayer()->object.GetPositionFloat();
	basePos = GetBase()->object.GetPositionFloat();

	// プレイヤーが使われていないのなら当たり判定も必要ないためまずプレイヤーの使用フラグ確認
	if (!player->use) return;

	// 弾丸毎の当たり判定----------------------------------------------------
	for (int b = 0; b < MAX_BULLET; b++)
	{
		// 弾丸の使用フラグを確認
		if (!bullet[b].use) continue;
		bulletPos = bullet[b].object.GetPositionFloat();

		// バレット対デブリ当たり判定
		for (int d = 0; d < MAX_DEBRIS; d++)
		{
			// デブリの使用フラグ確認
			if (!debris[d].use) continue;

			//使用フラグをチェックしたら座標をゲット
			debrisPos = debris[d].object.GetPositionFloat();

			// 弾丸とデブリの当たり判定
			if (!CollisionBC(bulletPos, debrisPos, bullet[b].size, debris[d].size)) continue;

			// 弾丸とデブリの当たり反応（真）
			bullet[b].spd = 0.0f;
			debris[d].flag_rotate = false;
			debris[d].object.SetParent(&bullet[b].object);
		}


		//バレットとアンテナの当たり判定
		for (int a = 0; a < MAX_ANTENNA; a++)
		{
			//アンテナの使用フラグをチェック
			if (!antenna[a].use)continue;
			//使用フラグをチェックしたら座標をゲット
			antennaPos = antenna[a].object.GetPositionFloat();

			// 弾丸とアンテナの当たり判定
			if (!CollisionBC(bulletPos, antennaPos, bullet[b].size, antenna[a].size))continue;

			bullet[b].spd = 0.0f;
			antenna[a].flag_rotate = false;
			antenna[a].object.SetParent(&bullet[b].object);

		}
	}
	//----------------------------------------------------------------------



	// プレイヤーの当たり判定-------------------------------------------------
	bool unload = false;		// 保持しているデブリを回収
	// プレイヤーと基地の当たり判定
	if (CollisionBC(playerPos, basePos, player->size, base->size))
	{
		// プレイヤーと基地の当たり反応（真
		if (!flag_score == 0)
		{
			PlaySound(SOUND_LABEL_SE_SCORE);
		}

		AddScore(flag_score * 100);
		flag_score = 0;
		for (int i = 0; i < MAX_BULLET; i++)
		{
			if (!bullet[i].spd == 1.0f)continue;
			{
				bullet[i].use = false;
			}
		}
	}
	// ペアレントしたバレットの解放処理
	for (int b = 0; b < MAX_BULLET; b++)
	{
		// 弾丸の使用フラグを確認
		if (!bullet[b].use)continue;

		// デブリとの処理
		for (int d = 0; d < MAX_DEBRIS; d++)
		{
			// デブリの使用フラグ確認
			if (!debris[d].use) continue;

			//使用フラグをチェックしたら座標をゲット
			debrisPos = debris[d].object.GetPositionFloat();

			// プレイヤーとデブリの当たり判定
			if (!CollisionBC(playerPos, debrisPos, player->size, debris[d].size)) continue;

			// プレイヤーとデブリの当たり反応（真）
			if (!debris[d].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				debris[d].use = false;
				flag_score += 1;

				if (debris[d].object.GetParent() == &bullet[b].object)
				{
					debris[d].object.SetParent(NULL);
					bullet[b].use = false;
					bullet[b].spd = 1.0f;
				}
				
			}
			//エフェクトのイメージは吸い込まれる感じ(マイクラの経験値が近い)
		}
		//----------------------------------------------------------------------

		
		// アンテナのと処理
		for (int a = 0; a < MAX_ANTENNA; a++)
		{
			//アンテナの使用フラグをチェック
			if (!antenna[a].use)continue;
			//使用フラグをチェックしたら座標をゲット
			antennaPos = antenna[a].object.GetPositionFloat();

			// プレイヤーとアンテナの当たり判定
			if (!CollisionBC(playerPos, antennaPos, player->size, antenna[a].size)) continue;
			// プレイヤーとアンテナの当たり反応（真）
			if (!antenna[a].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				antenna[a].use = false;
				flag_score += 5;


				if (antenna[a].object.GetParent() == &bullet[b].object)
				{
					antenna[a].object.SetParent(NULL);
					bullet[b].use = false;
					bullet[b].spd = 1.0f;
				}
			}
		}
		//----------------------------------------------------------------------

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
