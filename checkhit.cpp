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
#include "checkhit.h"



//*****************************************************************************
// グローバル変数
//*****************************************************************************

static int flag_score = 0;
static int flag_bonus = 0;

//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	// 各オブジェクトの位置を定義
	XMFLOAT3 podPos, basePos, hatchPos, panelPos, playerPos,
		debrisPos, bulletPos, rocketPos, antennaPos, satellitePos;

	//各オブジェクトのスケールを定義
	XMFLOAT3 playerScale, baseScale, bulletScale, debriScale,
		antennaScale, podScale, panelScale, hatchScale, satelliteScale, rocketScale;

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
	playerScale = GetPlayer()->object.GetScaleFloat();
	basePos = GetBase()->object.GetPositionFloat();
	baseScale = GetBase()->object.GetScaleFloat();
	// プレイヤーが使われていないのなら当たり判定も必要ないためまずプレイヤーの使用フラグ確認
	if (!player->use) return;

	// 弾丸毎の当たり判定----------------------------------------------------
	for (int cntBullet = 0; cntBullet < MAX_BULLET; cntBullet++)
	{
		// 弾丸の使用フラグを確認
		if (!bullet[cntBullet].use) continue;
		bulletPos = bullet[cntBullet].object.GetPositionFloat();
		bulletScale = bullet[cntBullet].object.GetScaleFloat();

		// バレット対デブリ当たり判定
		for (int cntDebris = 0; cntDebris < MAX_DEBRIS; cntDebris++)
		{
			// デブリの使用フラグ確認
			if (!debris[cntDebris].use) continue;

			//使用フラグをチェックしたら座標とスケールをゲット
			debrisPos = debris[cntDebris].object.GetPositionFloat();
			debriScale = debris[cntDebris].object.GetScaleFloat();
			// 弾丸とデブリの当たり判定
			if (!CheckDebris(bulletPos, bulletScale, debrisPos, debriScale)) continue;

			// 弾丸とデブリの当たり反応（真
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
			antennaScale = antenna[cntAntn].object.GetScaleFloat();

			// 弾丸とアンテナの当たり判定
			// X軸だけ長いためCheckLXを使用
			if (!CheckLX(bulletPos, bulletScale, antennaPos, antennaScale))continue;
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
			podScale = pod[cntPod].object.GetScaleFloat();

			// バレットとポッドの当たり判定
			if (!CheckDebris(bulletPos, bulletScale, podPos, podScale))continue;
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
			panelScale = panel[cntPanel].object.GetScaleFloat();

			// バレットとパネルの当たり判定
			if (!CheckDebris(bulletPos, bulletScale, panelPos, panelScale))continue;
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
			hatchScale = hatch[cntHatch].object.GetScaleFloat();

			// バレットとハッチの当たり判定
			if (!CheckDebris(bulletPos, bulletScale, hatchPos, hatchScale))continue;
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
			satelliteScale = satellite[cntSL].object.GetScaleFloat();

			// バレットと人工衛星の当たり判定
			// Z軸だけ長いためCheckLZを使用
			if (!CheckLZ(bulletPos, bulletScale, satellitePos, satelliteScale))continue;
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
			rocketScale = rocket[cntRocket].object.GetScaleFloat();

			// バレットとロケットの当たり判定
			if (!CheckLY(bulletPos, bulletScale, rocketPos, rocketScale))continue;
			bullet[cntBullet].spd = 0.0f;
			rocket[cntRocket].flag_rotate = false;
			rocket[cntRocket].object.SetParent(&bullet[cntBullet].object);
		}
	}

	//----------------------------------------------------------------------



	// プレイヤーの当たり判定-------------------------------------------------
	bool unload = false;		// 保持しているデブリを回収
	// プレイヤーと基地の当たり判定
	if (CheckBase(playerPos, playerScale, basePos, baseScale))
	{
		// プレイヤーと基地の当たり反応（真
		if (!flag_score == 0)
		{
			player->C2alpha = 1.0f;
			PlaySound(SOUND_LABEL_SE_SCORE);
			AddScore(flag_score * 100);
			AddBonus(flag_bonus * 50);
			flag_score = 0;
		}
		
		player->Ralpha = 1.0f;
		// Eキー押したら弾補充させる
		if (GetKeyboardTrigger(DIK_E))
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
	else
	{
		player->Ralpha = 0.0f;
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
			debriScale = debris[cntDebris].object.GetScaleFloat();

			// プレイヤーとデブリの当たり判定
			if (!CheckPlayer(playerPos, playerScale, debrisPos, debriScale)) continue;

			// プレイヤーとデブリの当たり反応（真）
			if (!debris[cntDebris].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				if (debris[cntDebris].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 1;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					debris[cntDebris].object.SetParent(NULL);
					debris[cntDebris].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 5.0f;
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
			antennaScale = antenna[cntAntn].object.GetScaleFloat();

			// プレイヤーとアンテナの当たり判定
			if (!CheckPlayer(playerPos, playerScale, antennaPos, antennaScale)) continue;

			// プレイヤーとアンテナの当たり反応（真）
			if (!antenna[cntAntn].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (antenna[cntAntn].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 5;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					antenna[cntAntn].object.SetParent(NULL);
					antenna[cntAntn].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 5.0f;
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
			podScale = pod[cntPod].object.GetScaleFloat();

			// プレイや―とポッドの当たり判定
			if (!CheckPlayer(playerPos, playerScale, podPos, podScale)) continue;

			// プレイや―とポッドの当たり判定(真)
			if (!pod[cntPod].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (pod[cntPod].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					pod[cntPod].object.SetParent(NULL);
					pod[cntPod].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 5.0f;
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
			panelScale = panel[cntPanel].object.GetScaleFloat();

			//プレイヤーとパネルの当たり判定
			if (!CheckPlayer(playerPos, playerScale, panelPos, panelScale)) continue;

			//プレイヤーとパネルの当たり判定(真)
			if (!panel[cntPanel].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (panel[cntPanel].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					panel[cntPanel].object.SetParent(NULL);
					panel[cntPanel].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 5.0f;
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
			hatchScale = hatch[cntHatch].object.GetScaleFloat();

			// プレイヤーとハッチの当たり判定
			if (!CheckPlayer(playerPos, hatchPos, playerScale, hatchScale)) continue;

			// プレイヤーとハッチの当たり判定(真)
			if (!hatch[cntHatch].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (hatch[cntHatch].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					hatch[cntHatch].object.SetParent(NULL);
					hatch[cntHatch].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 5.0f;
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
			satelliteScale = satellite[cntSL].object.GetScaleFloat();

			// プレイヤーと人工衛星の当たり判定
			if (!CheckPlayer(playerPos, playerScale, satellitePos, satelliteScale)) continue;

			// プレイヤーと人工衛星の当たり判定(真)
			if (!satellite[cntSL].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (satellite[cntSL].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					satellite[cntSL].object.SetParent(NULL);
					satellite[cntSL].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 5.0f;
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
			rocketScale = rocket[cntRocket].object.GetScaleFloat();

			// プレイヤーとロケットの当たり判定
			if (!CheckPlayer(playerPos, playerScale, rocketPos, rocketScale)) continue;

			// プレイヤーとロケットの当たり判定(真)
			if (!rocket[cntRocket].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (rocket[cntRocket].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					rocket[cntRocket].object.SetParent(NULL);
					rocket[cntRocket].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 5.0f;
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
// 拠点とプレイヤーの当たり判定の計算
//=============================================================================
BOOL CheckBase(XMFLOAT3 pos1, XMFLOAT3 scale1, XMFLOAT3 pos2, XMFLOAT3 scale2)
{
	BOOL ans = FALSE;		//はずれのセット
	
	// baseに合うようにスケールを調整
	scale1.x *= 20.0f;
	scale1.y *= 4.0f;
	scale1.z *= 2.0f;
	scale2.x *= 20.0f;
	scale2.y *= 4.0f;
	scale2.z *= 6.0f;

	if ((pos1.x + scale1.x > pos2.x - scale2.x) &&
		(pos1.x - scale1.x < pos2.x + scale2.x) &&
		(pos1.y + scale1.y > pos2.y - scale2.y) &&
		(pos1.y - scale1.y < pos2.y + scale2.y) &&
		(pos1.z + scale1.z > pos2.z - scale2.z) &&
		(pos1.z - scale1.z < pos2.z + scale2.z))
	{
		ans = TRUE;	// 当たっている
	}

	return ans;
}

//=============================================================================
// トリモチとデブリの当たり判定の計算
// 四角いやつはだいたいこれ
//=============================================================================
BOOL CheckPlayer(XMFLOAT3 ppos, XMFLOAT3 pscale, XMFLOAT3 dpos, XMFLOAT3 dscale)
{
	BOOL ans = FALSE;	// はずれのセット

	//素の数値だと小さすぎるので計算内だけスケールを増やす
	pscale.x *= 10.0f;
	pscale.y *= 10.0f;
	pscale.z *= 10.0f;
	dscale.x *= 10.0f;
	dscale.y *= 10.0f;
	dscale.z *= 10.0f;

	if ((ppos.x + pscale.x > dpos.x - dscale.x) &&
		(ppos.x - pscale.x < dpos.x + dscale.x) &&
		(ppos.y + pscale.y > dpos.y - dscale.y) &&
		(ppos.y - pscale.y < dpos.y + dscale.y) &&
		(ppos.z + pscale.z > dpos.z - dscale.z) &&
		(ppos.z - pscale.z < dpos.z + dscale.z))
	{
		ans = TRUE;
	}

	return ans;
}


//=============================================================================
// トリモチとデブリの当たり判定の計算
// 四角いやつはだいたいこれ
//=============================================================================
BOOL CheckDebris(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 dPos, XMFLOAT3 dScale)
{
	BOOL ans = FALSE;	// はずれのセット

	//素の数値だと小さすぎるので計算内だけスケールを増やす
	bScale.x *= 5.0f;
	bScale.y *= 5.0f;
	bScale.z *= 5.0f;
	dScale.x *= 5.0f;
	dScale.y *= 5.0f;
	dScale.z *= 5.0f;

	if ((bPos.x + bScale.x > dPos.x - dScale.x) &&
		(bPos.x - bScale.x < dPos.x + dScale.x) &&
		(bPos.y + bScale.y > dPos.y - dScale.y) &&
		(bPos.y - bScale.y < dPos.y + dScale.y) &&
		(bPos.z + bScale.z > dPos.z - dScale.z) &&
		(bPos.z - bScale.z < dPos.z + dScale.z))
	{
		ans = TRUE;
	}

	return ans;
}

//=============================================================================
// LongX(X軸が長いもの)の当たり判定
//=============================================================================
BOOL CheckLX(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 xPos, XMFLOAT3 xScale)
{
	BOOL ans = FALSE;	// はずれのセット

	// x軸に長いのでそれを調節
	bScale.x *= 10.0f;
	bScale.y *= 3.0f;
	bScale.z *= 3.0f;
	xScale.x *= 10.0f;
	xScale.y *= 3.0f;
	xScale.z *= 3.0f;

	if ((bPos.x + bScale.x > xPos.x - xScale.x) &&
		(bPos.x - bScale.x < xPos.x + xScale.x) &&
		(bPos.y + bScale.y > xPos.y - xScale.y) &&
		(bPos.y - bScale.y < xPos.y + xScale.y) &&
		(bPos.z + bScale.z > xPos.z - xScale.z) &&
		(bPos.z - bScale.z < xPos.z + xScale.z))
	{
		ans = TRUE;
	}

	return ans;
}

//=============================================================================
// LongY(Y軸が長いもの)の当たり判定
//=============================================================================
BOOL CheckLY(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 yPos, XMFLOAT3 yScale)
{
	BOOL ans = FALSE;	// はずれのセット

	// x軸に長いのでそれを調節
	bScale.x *= 3.0f;
	bScale.y *= 10.0f;
	bScale.z *= 3.0f;
	yScale.x *= 3.0f;
	yScale.y *= 10.0f;
	yScale.z *= 3.0f;

	if ((bPos.x + bScale.x > yPos.x - yScale.x) &&
		(bPos.x - bScale.x < yPos.x + yScale.x) &&
		(bPos.y + bScale.y > yPos.y - yScale.y) &&
		(bPos.y - bScale.y < yPos.y + yScale.y) &&
		(bPos.z + bScale.z > yPos.z - yScale.z) &&
		(bPos.z - bScale.z < yPos.z + yScale.z))
	{
		ans = TRUE;
	}

	return ans;
}


//=============================================================================
// LongZ(Z軸が長いもの)の当たり判定
//=============================================================================
BOOL CheckLZ(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 zPos, XMFLOAT3 zScale)
{
	BOOL ans = FALSE;	// はずれのセット

	// z軸に長いのでそれを調節
	bScale.x *= 3.0f;
	bScale.y *= 3.0f;
	bScale.z *= 10.0f;
	zScale.x *= 3.0f;
	zScale.y *= 3.0f;
	zScale.z *= 10.0f;

	if ((bPos.x + bScale.x > zPos.x - zScale.x) &&
		(bPos.x - bScale.x < zPos.x + zScale.x) &&
		(bPos.y + bScale.y > zPos.y - zScale.y) &&
		(bPos.y - bScale.y < zPos.y + zScale.y) &&
		(bPos.z + bScale.z > zPos.z - zScale.z) &&
		(bPos.z - bScale.z < zPos.z + zScale.z))
	{
		ans = TRUE;
	}

	return ans;
}