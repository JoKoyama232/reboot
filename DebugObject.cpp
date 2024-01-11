//=============================================================================
// デバッグオブジェクト処理 [DebugObj.cpp]
// Author : 小山　城
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "WorldSpaceClass.h"
#include "debugproc.h"
#include "DebugObject.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_DebugObj_Sphere		"Data/Model/debugDistance.obj"	// 読み込むモデル名
#define DebugObj_AMNT_MAX		(3)					// ロケットの最大描画数+1　(ループ処理の為)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static DebugObj g_DebugObj[DebugObj_AMNT_MAX];	// ロケットの配列

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDebugObj(void)
{
	for (int idx = 0; idx < DebugObj_AMNT_MAX; idx++) {

		LoadModel(MODEL_DebugObj_Sphere, &g_DebugObj[idx].transform.modelInfo);		// ロケットのモデル設定
		g_DebugObj[idx].transform.load = false;					// エラーがなければモデル読み込み成功
		g_DebugObj[idx].transform.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));	// デフォルトの位置
		g_DebugObj[idx].transform.SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));	// デフォルトの回転
		g_DebugObj[idx].transform.SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));		// デフォルトのスケール
	}
	// 全てのロケット分の初期数値が設定されたため初期化成功
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDebugObj(void)
{
	for (int idx = 0; idx < DebugObj_AMNT_MAX; idx++) {
		// モデルの解放処理
		if (g_DebugObj[idx].transform.load == true)
		{
			UnloadModel(&g_DebugObj[idx].transform.modelInfo);
			g_DebugObj[idx].transform.load = false;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDebugObj(void)
{
	// 更新は各自の対象のクラスで行う
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDebugObj(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix, mtxObj;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int idx = 0; idx < DebugObj_AMNT_MAX; idx++) {
		// 使用されていないロケットは描画を行わない
		if (!g_DebugObj[idx].use) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		//入力情報にスケールをゲット
		XMFLOAT3 input = g_DebugObj[idx].transform.GetScaleFloat();
		// スケールを反映
		mtxScl = XMMatrixScaling(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		//入力情報に回転をゲット
		input = g_DebugObj[idx].transform.GetRotationFloat();
		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		//入力情報に移動をゲット
		input = g_DebugObj[idx].transform.GetPositionFloat();
		// 移動を反映
		mtxTranslate = XMMatrixTranslation(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		// ワールドマトリックスの保存
		XMStoreFloat4x4(g_DebugObj[idx].transform.GetWorldMatrixPointer(), mtxWorld);

		// モデル描画
		DrawModel(&g_DebugObj[idx].transform.modelInfo);
	}
	XMFLOAT3 position = g_DebugObj[0].transform.GetPositionFloat();
	XMFLOAT3 rotation = g_DebugObj[0].transform.GetScaleFloat();
	PrintDebugProc((char*)"Debug Information\nPosition:(%f, %f, %f)\nScale:(%f, %f, %f)\n", position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);

	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// デバッグオブジェクト生成
//=============================================================================
int SetDebugObj(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
{
	for (int idx = 0; idx < DebugObj_AMNT_MAX; idx++) 
	{

		// 最初のオブジェクトが使用されていなければループ
		if (idx == DebugObj_AMNT_MAX - 1 && !g_DebugObj[0].use) {
			idx = 0;
		}

		// 使用されていれば次へ行く
		if (g_DebugObj[idx].use) continue;

		g_DebugObj[idx].transform.SetPosition(position);
		g_DebugObj[idx].transform.SetRotation(rotation);
		g_DebugObj[idx].transform.SetScale(scale);
		g_DebugObj[idx].use = true;
		return idx;
	}
	// 失敗時に-1を戻す
	return -1;
}

//=============================================================================
// デバッグオブジェクトの情報を取得
//=============================================================================
DebugObj* GetDebugObj(int idx)
{
	return &g_DebugObj[idx];
}