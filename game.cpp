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
#include "playerUI.h"
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
#include "basearrow.h"

#include "game.h"
#include "checkhit.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



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

	InitPlayerUI();

	InitDebris();

	InitAntenna();

	InitPanel();

	InitHatch();

	InitPod();

	InitSatellite();

	InitRocket();

	InitBase();

	InitBullet();

	InitClock();

	InitScore();

	InitSkybox();

	InitArrow();

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

	UninitBullet();

	UninitClock();

	UninitScore();

	UninitArrow();
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

	UpdateBullet();

	UpdateClock();

	UpdateSkybox();

	UpdateArrow();

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

	DrawBullet();

	DrawArrow();

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

		DrawAttach();

		DrawCapture();

		DrawCollect();

		DrawReload();

		DrawReticle();

		DrawHit();

		DrawMeterBase();

		// ライティングを有効に
		SetLightEnable(true);

		// Z比較あり
		SetDepthEnable(true);
	}

}

