// プレイヤー処理 [Player.cpp]
// Author : 小山　城　小井詰　礼
//=============================================================================
#include "model.h"
#include "main.h"

#include "Player.h"
#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER		"Data/model/cone.obj"			// 読み込むモデル名(まだ存在してないよ)

#define	VALUE_MOVE			(2.0f)							// 移動量

static float		roty = 0.0f;


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static float        g_LastUpdate;
PLAYER g_Player;
static bool g_fire = false;

HRESULT InitPlayer(void) {

		LoadModel(MODEL_PLAYER, &g_Player.object.modelInfo);
		GetModelDiffuse(&g_Player.object.modelInfo, g_Player.object.modelDiffuse);
		g_Player.object.load = true;

		g_Player.object.SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Player.object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Player.object.SetScale(XMFLOAT3{ 1.0f, 1.0f, 1.0f });
		g_Player.time = 0.0f;
		g_Player.spd = 0.0f;			// 移動スピードクリア
		g_Player.use = true;
		g_LastUpdate = 0.0f;
		roty = 0.0f;

		return S_OK;

}

void UninitPlayer(void) {
	// モデルの解放処理
	if (g_Player.object.load)
	{
		UnloadModel(&g_Player.object.modelInfo);
		g_Player.object.load = false;
	}

}

void UpdatePlayer(void) {

	DWORD updateTime = timeGetTime();
	DWORD deltaTime = updateTime - g_LastUpdate;
	g_LastUpdate = updateTime;

	// 移動処理
	if (GetKeyboardPress(DIK_A))
	{
		g_Player.spd = VALUE_MOVE;
		roty = XM_PI / 2;
	}
	else if (IsButtonTriggered(0, BUTTON_LEFT))
	{
		g_Player.spd = VALUE_MOVE;
		roty = XM_PI / 2;

	}

	if (GetKeyboardPress(DIK_D))
	{
		g_Player.spd = VALUE_MOVE;
		roty = -XM_PI / 2;
	}
	else if (IsButtonTriggered(0, BUTTON_RIGHT))
	{
		g_Player.spd = VALUE_MOVE;
		roty = -XM_PI / 2;
	}

	if ((GetKeyboardPress(DIK_SPACE)) || IsMouseLeftTriggered())
	{
		g_Player.object.SetPosition(XMFLOAT3{ 1.0f, 0.0f, 0.0f });
		g_Player.spd = VALUE_MOVE;
		roty = XM_PI;
	}
	//else if (IsButtonTriggered(0, BUTTON_UP))
	//{
	//	g_Player.spd = VALUE_MOVE;
	//	roty = XM_PI;
	//}
	else
	{
		g_Player.object.SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
	}

	if (GetKeyboardPress(DIK_S))
	{
		g_Player.spd = VALUE_MOVE;
		roty = 0.0f;
	}
	else if (IsButtonTriggered(0, BUTTON_DOWN))
	{
		g_Player.spd = VALUE_MOVE;
		roty = 0.0f;
	}

	// 弾発射処理

	XMFLOAT3 p_pos = g_Player.object.GetPositionFloat();
	XMFLOAT3 p_rot = g_Player.object.GetRotationFloat();
	if (GetKeyboardTrigger(DIK_SPACE)/* && updateTime - g_LastUpdate > 3000*/)
	{
 		SetBullet(p_pos,p_rot);
		//g_LastUpdate = updateTime;
		
	}
	//if (GetKeyboardPress(DIK_W))
	//{
	//	SetBullet(p_pos, p_rot);
	//}
	//else if (IsButtonTriggered(0, BUTTON_A))
	//{

	//}

	PrintDebugProc((char*)"Player Information\nMovement:   W\n            A  S  D\n  Shift    Space\nPosition:(%f, %f, %f)\nRotation:(%f, %f, %f)\n", p_pos.x, p_pos.y, p_pos.z, p_rot.x, p_rot.y, p_rot.z);

}

void DrawPlayer(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	XMFLOAT3 scale = g_Player.object.GetScaleFloat();
	mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	XMFLOAT3 rotation = g_Player.object.GetRotationFloat();
	mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


	// 移動を反映
	XMFLOAT3 position = g_Player.object.GetPositionFloat();
	mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	
	XMStoreFloat4x4(g_Player.object.GetWorldMatrixPointer(), mtxWorld);


	// 縁取りの設定
	SetFuchi(1);

	// モデル描画
	DrawModel(&g_Player.object.modelInfo);

	SetFuchi(0);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);

}

PLAYER* GetPlayer(void) {
	return &g_Player;
}

XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage) {
	XMFLOAT3 result;
	XMVECTOR pos0 = XMLoadFloat3(&initialPos);
	XMVECTOR pos1 = XMLoadFloat3(&endingPos);

	pos0 += (pos1 - pos0) * percentage;
	XMStoreFloat3(&result, pos0);
	return result;
}

float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2) {
	return (float)sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y) + (pos1.z - pos2.z) * (pos1.z - pos2.z));
}