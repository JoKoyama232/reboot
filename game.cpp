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

	DrawPod();

	DrawPanel();

	DrawHatch();

	DrawSatellite();

	DrawRocket();

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
		debrisPos, bulletPos, rocketPos, antennaPos, satellitePos;

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
	for (int cntBullet = 0; cntBullet < MAX_BULLET; cntBullet++)
	{
		// 弾丸の使用フラグを確認
		if (!bullet[cntBullet].use) continue;
		bulletPos = bullet[cntBullet].object.GetPositionFloat();

		// バレット対デブリ当たり判定
		for (int cntDebris = 0; cntDebris < MAX_DEBRIS; cntDebris++)
		{
			// デブリの使用フラグ確認
			if (!debris[cntDebris].use) continue;

			//使用フラグをチェックしたら座標をゲット
			debrisPos = debris[cntDebris].object.GetPositionFloat();

			// 弾丸とデブリの当たり判定
			if (!CollisionBC(bulletPos, debrisPos, bullet[cntBullet].size, debris[cntDebris].size)) continue;

			// 弾丸とデブリの当たり反応（真）
			bullet[cntBullet].spd = 0.0f;
			debris[cntDebris].flag_rotate = false;
			debris[cntDebris].object.SetParent(&bullet[cntBullet].object);
		}


		//バレットとアンテナの当たり判定
		for (int cntAntn = 0; cntAntn < MAX_ANTENNA; cntAntn++)
		{
			//アンテナの使用フラグをチェック
			if (!antenna[cntAntn].use)continue;

			//使用フラグをチェックしたら座標をゲット
			antennaPos = antenna[cntAntn].object.GetPositionFloat();

			// 弾丸とアンテナの当たり判定
			if (!CollisionBC(bulletPos, antennaPos, bullet[cntBullet].size, antenna[cntAntn].size))continue;

			bullet[cntBullet].spd = 0.0f;
			antenna[cntAntn].flag_rotate = false;
			antenna[cntAntn].object.SetParent(&bullet[cntBullet].object);
		}

		// バレットとポッドの当たり判定
		for (int cntPod = 0; cntPod < MAX_POD; cntPod++)
		{
			// ポッドの仕様フラグをチェック
			if (!pod[cntPod].use)continue;

			//仕様フラグをチェックしたら座標をゲット
			podPos = pod[cntPod].object.GetPositionFloat();

			// バレットとポッドの当たり判定
			if (!CollisionBC(bulletPos, podPos, bullet[cntBullet].size, pod[cntPod].size))continue;

			bullet[cntBullet].spd = 0.0f;
			pod[cntPod].flag_rotate = false;
			pod[cntPod].object.SetParent(&bullet[cntBullet].object);
		}

		// バレットとソ－ラーパネルの当たり判定
		for (int cntPanel = 0; cntPanel < MAX_PANEL; cntPanel++)
		{
			// ソーラーパネルの仕様フラグをチェック
			if (!panel[cntPanel].use)continue;
			
			// 仕様フラグをチェックしたら座標をゲット
			panelPos = panel[cntPanel].object.GetPositionFloat();

			// バレットとパネルの当たり判定
			if (!CollisionBC(bulletPos, panelPos, bullet[cntBullet].size, panel[cntPanel].size))continue;
		
			bullet[cntBullet].spd = 0.0f;
			panel[cntPanel].flag_rotate = false;
			panel[cntPanel].object.SetParent(&bullet[cntBullet].object);		
		}

		// バレットとハッチの当たり判定
		for (int cntHatch = 0; cntHatch < MAX_HATCH; cntHatch++)
		{
			// ハッチに仕様フラグをチェック
			if (!hatch[cntHatch].use)continue;
			
			// 仕様フラグをチェックしたら座標をゲット
			hatchPos = hatch[cntHatch].object.GetPositionFloat();
			
			// バレットとハッチの当たり判定
			if (!CollisionBC(bulletPos, hatchPos, bullet[cntBullet].size, hatch[cntHatch].size))continue;
			
			bullet[cntBullet].spd = 0.0f;
			hatch[cntHatch].flag_rotate = false;
			hatch[cntHatch].object.SetParent(&bullet[cntBullet].object);
			
		}

		// バレットと人工衛星の当たり判定
		for (int cntSL = 0; cntSL < MAX_SATELLITE; cntSL++)
		{
			// 人工衛星の使用フラグをチェック
			if (!satellite[cntSL].use)continue;

			// 仕様フラグをチェックしたら座標をゲット
			satellitePos = satellite[cntSL].object.GetPositionFloat();

			// バレットと人工衛星の当たり判定
			if (!CollisionBC(bulletPos, satellitePos, bullet[cntBullet].size, satellite[cntSL].size))continue;

			bullet[cntBullet].spd = 0.0f;
			satellite[cntSL].flag_rotate = false;
			satellite[cntSL].object.SetParent(&bullet[cntBullet].object);
		}
		
		// バレットとロケットの当たり判定
		for (int cntRocket = 0; cntRocket < MAX_ROCKET; cntRocket++)
		{
			// ロケットの仕様フラグをチェック
			if (!rocket[cntRocket].use)continue;

			// 仕様フラグをチェックしたら座標をゲット
			rocketPos = rocket[cntRocket].object.GetPositionFloat();

			// バレットとロケットの当たり判定
			if (!CollisionBC(bulletPos, rocketPos, bullet[cntBullet].size, rocket[cntRocket].size))continue;

			bullet[cntBullet].spd = 0.0f;
			rocket[cntRocket].flag_rotate = false;
			rocket[cntRocket].object.SetParent(&bullet[cntBullet].object);
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

		// Eキー押したら弾補充させる
		//if (GetKeyboardTrigger(DIK_E))
		{
			for (int i = 0; i < MAX_BULLET; i++)
			{
				if (!bullet[i].spd == 1.0f)continue;
				{
					bullet[i].use = false;
				}
			}
		}
	}


	// ペアレントしたバレットの解放処理
	for (int cntBullet = 0; cntBullet < MAX_BULLET; cntBullet++)
	{
		// 弾丸の使用フラグを確認
		if (!bullet[cntBullet].use)continue;

		// デブリとの処理
		for (int cntDebris = 0; cntDebris < MAX_DEBRIS; cntDebris++)
		{
			// デブリの使用フラグ確認
			if (!debris[cntDebris].use) continue;

			//使用フラグをチェックしたら座標をゲット
			debrisPos = debris[cntDebris].object.GetPositionFloat();

			// プレイヤーとデブリの当たり判定
			if (!CollisionBC(playerPos, debrisPos, player->size, debris[cntDebris].size)) continue;

			// プレイヤーとデブリの当たり反応（真）
			if (!debris[cntDebris].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 1;

				if (debris[cntDebris].object.GetParent() == &bullet[cntBullet].object)
				{
					debris[cntDebris].object.SetParent(NULL);
					debris[cntDebris].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 2.0f;
				}
				
			}
			//エフェクトのイメージは吸い込まれる感じ(マイクラの経験値が近い)
		}
		//----------------------------------------------------------------------

		
		// アンテナのと処理
		for (int cntAntn = 0; cntAntn < MAX_ANTENNA; cntAntn++)
		{
			//アンテナの使用フラグをチェック
			if (!antenna[cntAntn].use)continue;
			//使用フラグをチェックしたら座標をゲット
			antennaPos = antenna[cntAntn].object.GetPositionFloat();

			// プレイヤーとアンテナの当たり判定
			if (!CollisionBC(playerPos, antennaPos, player->size, antenna[cntAntn].size)) continue;
			
			// プレイヤーとアンテナの当たり反応（真）
			if (!antenna[cntAntn].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 5;

				if (antenna[cntAntn].object.GetParent() == &bullet[cntBullet].object)
				{
					antenna[cntAntn].object.SetParent(NULL);
					antenna[cntAntn].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 2.0f;
				}
			}
		}
		//----------------------------------------------------------------------



		// ポッドとの処理
		for (int cntPod = 0; cntPod < MAX_POD; cntPod++)
		{
			// ポッドの使用フラグをチェック
			if (!pod[cntPod].use)continue;
			//仕様フラグを確認したら座標をゲット
			podPos = pod[cntPod].object.GetPositionFloat();

			// プレイや―とポッドの当たり判定
			if (!CollisionBC(playerPos, podPos, player->size, pod[cntPod].size)) continue;
			
			// プレイや―とポッドの当たり判定(真)
			if (!pod[cntPod].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 3;

				if (pod[cntPod].object.GetParent() == &bullet[cntBullet].object)
				{
					pod[cntPod].object.SetParent(NULL);
					pod[cntPod].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 2.0f;
				}
			}
		}
		//----------------------------------------------------------------------


		// ソーラーパネルとの処理
		for (int cntPanel = 0; cntPanel < MAX_PANEL; cntPanel++)
		{
			// パネルの仕様フラグをチェック
			if (!panel[cntPanel].use)continue;
			// 仕様フラグをチェックしたら座標をゲット
			panelPos = panel[cntPanel].object.GetPositionFloat();

			//プレイヤーとパネルの当たり判定
			if (!CollisionBC(playerPos, panelPos, player->size, panel[cntPanel].size)) continue;
			
			//プレイヤーとパネルの当たり判定(真)
			if (!panel[cntPanel].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 3;

				if (panel[cntPanel].object.GetParent() == &bullet[cntBullet].object)
				{
					panel[cntPanel].object.SetParent(NULL);
					panel[cntPanel].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 2.0f;
				}
			}
		}
		//----------------------------------------------------------------------


		// ハッチとの処理
		for (int cntHatch = 0; cntHatch < MAX_HATCH; cntHatch++)
		{
			// ハッチの仕様フラグをチェック
			if (!hatch[cntHatch].use)continue;

			// 仕様フラグをチェックしたら座標をゲット
			hatchPos = hatch[cntHatch].object.GetPositionFloat();

			// プレイヤーとハッチの当たり判定
			if (!CollisionBC(playerPos, hatchPos, player->size, hatch[cntHatch].size)) continue;
		
			// プレイヤーとハッチの当たり判定(真)
			if (!hatch[cntHatch].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 3;

				if (hatch[cntHatch].object.GetParent() == &bullet[cntBullet].object)
				{
					hatch[cntHatch].object.SetParent(NULL);
					hatch[cntHatch].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 2.0f;
				}
			}
		}
		//----------------------------------------------------------------------


		// 人工衛星との処理
		for (int cntSL = 0; cntSL < MAX_SATELLITE; cntSL++)
		{
			// 人工衛星の使用フラグをチェック
			if (!satellite[cntSL].use)continue;

			// 仕様フラグをチェックしたら座標をゲット
			satellitePos = satellite[cntSL].object.GetPositionFloat();

			// プレイヤーと人工衛星の当たり判定
			if (!CollisionBC(playerPos, satellitePos, player->size, satellite[cntSL].size)) continue;

			// プレイヤーと人工衛星の当たり判定(真)
			if (!satellite[cntSL].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 3;

				if (satellite[cntSL].object.GetParent() == &bullet[cntBullet].object)
				{
					satellite[cntSL].object.SetParent(NULL);
					satellite[cntSL].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 2.0f;
				}
			}
		}
		//----------------------------------------------------------------------


		for (int cntRocket = 0; cntRocket < MAX_ROCKET; cntRocket++)
		{
			// ロケットの使用フラグをチェック
			if (!rocket[cntRocket].use)continue;

			// 使用フラグをチェックしたら座標をゲット
			rocketPos = rocket[cntRocket].object.GetPositionFloat();

			// プレイヤーとロケットの当たり判定
			if (!CollisionBC(playerPos, rocketPos, player->size, rocket[cntRocket].size)) continue;

			// プレイヤーとロケットの当たり判定(真)
			if (!rocket[cntRocket].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 3;

				if (rocket[cntRocket].object.GetParent() == &bullet[cntBullet].object)
				{
					rocket[cntRocket].object.SetParent(NULL);
					rocket[cntRocket].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 2.0f;
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


//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらTRUE
//=============================================================================
BOOL CollisionBC2(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 s1, XMFLOAT3 s2)
{
	BOOL ans = FALSE;						// 外れをセットしておく

	XMFLOAT3 len = { (s1.x + s2.x) * (s1.x + s2.x), (s1.y + s2.y) * (s1.y + s2.y), (s1.z + s2.z) * (s1.z + s2.z)};
	XMVECTOR temp = XMLoadFloat3(&pos1) - XMLoadFloat3(&pos2);
	temp = XMVector3LengthSq(temp);			// 2点間の距離（2乗した物）
	XMFLOAT3 lenSq = XMFLOAT3{ 0.0f,0.0f,0.0f };

	if ((len.x > lenSq.x) || (len.y > lenSq.y) || (len.z > lenSq.z))
	{
		ans = TRUE;	// 当たっている
	}


	return ans;
}
