// プレイヤー処理 [Player.cpp]
// Author : 小山　城　小井詰　礼
//=============================================================================
#include "model.h"
#include "main.h"

#include "Player.h"
#include "input.h"
#include "debugproc.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER		"Data/model/cone.obj"			// 読み込むモデル名(まだ存在してないよ)

//*****************************************************************************
// グローバル変数
//*****************************************************************************

Player g_Player;

HRESULT InitPlayer(void) {

		LoadModel(MODEL_PLAYER, &g_Player.object.modelInfo);
		GetModelDiffuse(&g_Player.object.modelInfo, g_Player.object.modelDiffuse);
		g_Player.object.load = true;

		g_Player.object.SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Player.object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Player.object.SetScale(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Player.time = 0.0f;
		g_Player.spd = 0.0f;			// 移動スピードクリア
		g_Player.use = true;

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

	// 移動処理
	if (GetKeyboardPress(DIK_A))
	{

		g_Player.object.SetRotation(XMFLOAT3{ 0.0f,90.0f,0.0f});
	}
	else if (IsButtonTriggered(0, BUTTON_LEFT))
	{

		g_Player.object.SetRotation(XMFLOAT3{ 0.0f,90.0f,0.0f });
	}

	if (GetKeyboardPress(DIK_D))
	{

		g_Player.object.SetRotation(XMFLOAT3{ 0.0f,-90.0f,0.0f });
	}
	else if (IsButtonTriggered(0, BUTTON_RIGHT))
	{

		g_Player.object.SetRotation(XMFLOAT3{ 0.0f,-90.0f,0.0f });
	}

	if (GetKeyboardPress(DIK_W))
	{

		g_Player.object.SetRotation(XMFLOAT3{ 0.0f, 180.0f,0.0f });
	}
	else if (IsButtonTriggered(0, BUTTON_UP))
	{

		g_Player.object.SetRotation(XMFLOAT3{ 0.0f, 180.0f,0.0f });
	}

	if (GetKeyboardPress(DIK_S))
	{

		g_Player.object.SetRotation(XMFLOAT3{ 0.0f, 0.0f,0.0f });
	}
	else if (IsButtonTriggered(0, BUTTON_DOWN))
	{

		g_Player.object.SetRotation(XMFLOAT3{ 0.0f, 0.0f,0.0f });
	}

	// 弾発射処理
	if (GetKeyboardTrigger(DIK_RETURN))
	{

	}
	else if (IsButtonTriggered(0, BUTTON_A))
	{

	}



}

void DrawPlayer(void) {



}

Player* GetPlayer(void) {
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