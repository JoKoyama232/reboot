// 背景処理 [Skybox.h]
// Author : 小山　城
//=============================================================================
#pragma once
// インクルード
#include "model.h"
#include "WorldSpaceClass.h"


// 構造定義
struct Skybox {
	// ワールド位置情報
	WorldSpace transform;

	// 読み込み状況管理
	bool use = false;
	bool load = false;

};

// 関数群
HRESULT InitSkybox(void);
void UninitSkybox(void);
void UpdatSkybox(void);
void DrawSkybox(void);