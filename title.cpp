//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "sprite.h"
#include "title.h"
#include "title_tex.h"
#include "title_debris.h"
#include "title_antenna.h"
#include "Skybox.h"

//=============================================================================
// 初期化処理
//=============================================================================

HRESULT InitTitle(void)
{
	InitTitleTex();

	InitTitleDebris();

	InitTitleAntenna();

	InitSkybox();
	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	UninitTitleTex();

	UninitTitleDebris();

	UninitTitleAntenna();

	UninitSkybox();
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	UpdateTitleTex();

	UpdateTitleDebris();

	UpdateTitleAntenna();

	UpdateSkybox();
}



//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	DrawTitleDebris();

	DrawTitleAntenna();

	DrawSkybox();

	// 2Dの物を描画する処理
	{
		// Z比較なし
		SetDepthEnable(false);

		// ライティングを無効
		SetLightEnable(false);

		//ここから下に
		DrawTitleTex();

		// ライティングを有効に
		SetLightEnable(true);

		// Z比較あり
		SetDepthEnable(true);
	}
}