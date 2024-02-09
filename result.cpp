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
#include "result.h"
#include "result_tex.h"
#include "Skybox.h"

//=============================================================================
// 初期化処理
//=============================================================================

HRESULT InitResult(void)
{

	InitResultTex();

	InitSkybox();

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	UninitResultTex();

	UninitSkybox();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	UpdateResultTex();

	UpdateSkybox();

#ifdef _DEBUG
	if (GetKeyState(VK_RBUTTON) & 0x80)
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
#endif // _DEBUG

}



//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	DrawSkybox();

	// 2Dの物を描画する処理
	{
		// Z比較なし
		SetDepthEnable(false);

		// ライティングを無効
		SetLightEnable(false);

		//ここから下に
		DrawResultTex();

		// ライティングを有効に
		SetLightEnable(true);

		// Z比較あり
		SetDepthEnable(true);
	}
}