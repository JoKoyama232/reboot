// 背景処理 [Skybox.cpp]
// Author : 小山　城
//=============================================================================

// インクルード

// 自身のインクルード
#include "Skybox.h"

// マクロ定義
#define MODEL_SKYBOX "data/model/skybox.obj"

// グローバル変数
static Skybox g_Skybox;

// 初期処理
HRESULT InitSkybox(void) {
	g_Skybox.transform.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	g_Skybox.transform.SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	g_Skybox.transform.SetScale(XMFLOAT3(0.0f, 0.0f, 0.0f));

	g_Skybox.load = true;

	return S_OK;
}

// 終了処理