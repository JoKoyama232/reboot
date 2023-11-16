// 背景処理 [Skybox.cpp]
// Author : 小山　城
//=============================================================================

// インクルード
#include "model.h"
#include "WorldSpaceClass.h"

// 自身のインクルード
#include "Skybox.h"

// マクロ定義
enum sky {
	SkyBG = 0,
	CloudBG,
	Clouds,
	SkyObjectMax
};
		
// グローバル変数
static WorldSpace g_Sky[3];		// 空のオブジェクト（独自変数がいらないためモデルクラスを使用）

static char* skyModelAdr[3]{	// 空のオブジェクトのモデルアドレス
	(char*)"Data/Model/Skysphere.obj",	// 空の背景球体
	(char*)"Data/Model/CloudBG.obj",	// 上下の雲
	(char*)"Data/Model/Cloud.obj"		// 横に回転している雲
};

// 初期処理
HRESULT InitSkybox(void) {
	for (int idx = 0; idx < SkyObjectMax; idx++) {
		// モデルの初期化
		LoadModel(skyModelAdr[idx], &g_Sky[idx].modelInfo);
		GetModelDiffuse(&g_Sky[idx].modelInfo, g_Sky[idx].modelDiffuse);
		g_Sky[idx].load = true;		
		// モデルの初期化完了

		// 変数初期化
		g_Sky[idx].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_Sky[idx].SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_Sky[idx].SetScale(XMFLOAT3(15.0f, 15.0f, 15.0f));

	}
	g_Sky[CloudBG].SetScale(XMFLOAT3(190.0f, 190.0f, 190.0f));
	g_Sky[Clouds].SetScale(XMFLOAT3(190.0f, 190.0f, 190.0f));
	return S_OK;
}

// 終了処理
void UninitSkybox(void) {
	for (int idx = 0; idx < SkyObjectMax; idx++) {
		if (!g_Sky[idx].load) continue;

		UnloadModel(&g_Sky[idx].modelInfo);
		g_Sky[idx].load = false;
		
	}
}

void UpdateSkybox(void) {
	// 空の回転処理
	XMFLOAT3 rotation = g_Sky[SkyBG].GetRotationFloat();
	rotation.y += XM_PI * 0.0005f;
	if (rotation.y > XM_PI * 2.0f) {
		rotation.y - XM_PI * 2.0f;
	}
	g_Sky[SkyBG].SetRotation(rotation);

	// 変数rotationは雲の背景回転に再定義
	rotation = g_Sky[CloudBG].GetRotationFloat();
	rotation.y += XM_PI * 0.0007f;
	if (rotation.y > XM_PI * 2.0f) {
		rotation.y - XM_PI * 2.0f;
	}
	g_Sky[CloudBG].SetRotation(rotation);

	// 変数rotationは雲の背景回転に再定義
	rotation = g_Sky[Clouds].GetRotationFloat();
	rotation.y += XM_PI * 0.001f;
	if (rotation.y > XM_PI * 2.0f) {
		rotation.y - XM_PI * 2.0f;
	}
	g_Sky[Clouds].SetRotation(rotation);
}

void DrawSkybox(void) {
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// BACKカリング　（球体の中にカメラがあるため）
	SetCullingMode(CULL_MODE_BACK);

	// 光を無効
	SetLightEnable(false);

	for (int idx = 0; idx < SkyObjectMax; idx++) {
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		//入力情報にスケールをゲット
		XMFLOAT3 input = g_Sky[idx].GetScaleFloat();

		// スケールを反映
		mtxScl = XMMatrixScaling(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		//入力情報に回転をゲット
		input = g_Sky[idx].GetRotationFloat();

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		//入力情報に移動をゲット
		input = g_Sky[idx].GetPositionFloat();

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		// ワールドマトリックスの保存
		XMStoreFloat4x4(g_Sky[idx].GetWorldMatrixPointer(), mtxWorld);

		// モデル描画
		DrawModel(&g_Sky[idx].modelInfo);
	}

	// 光を有効有効にする
	SetLightEnable(true);

	// カリングをBACKに戻す
	SetCullingMode(CULL_MODE_BACK);
}