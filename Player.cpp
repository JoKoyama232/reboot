// プレイヤー処理 [Player.cpp]
// Author : 小山　城　小井詰　礼
//=============================================================================
#include "model.h"
#include "main.h"
#include "camera.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "Player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER		"Data/model/cone.obj"			// 読み込むモデル名(まだ存在してないよ)

#define	VALUE_MOVE			(1.0f)							// 移動量

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
		g_Player.speed = 0.0f;			// 移動スピードクリア
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

	// プレイヤー変数取得
	XMFLOAT3 position = g_Player.object.GetPositionFloat();
	XMFLOAT3 rotation = g_Player.object.GetRotationFloat();

	// ファイル外変数の取得
	DWORD updateTime = timeGetTime();
	DWORD deltaTime = (updateTime - g_LastUpdate) * 0.1;
	g_LastUpdate = updateTime;
	CAMERA* cam = GetCamera();
	XMFLOAT3 camPosition = cam->pos;
	XMFLOAT3 camRotation = cam->rot;

	// 入力検知
	if (GetKeyboardPress(DIK_A) || IsButtonTriggered(0, BUTTON_LEFT))
	{	// 左
		g_Player.speed = VALUE_MOVE;
		g_Player.direction = -XM_PI / 2;
	}
	else if (GetKeyboardPress(DIK_D) || IsButtonTriggered(0, BUTTON_RIGHT))
	{	// 右
		g_Player.speed = VALUE_MOVE;
		g_Player.direction = XM_PI / 2;
	}
	else {
		g_Player.direction = 0.0f;
	}

	if (GetKeyboardPress(DIK_W) || IsButtonTriggered(0, BUTTON_UP))
	{	// 前
		g_Player.speed = VALUE_MOVE;
		g_Player.direction = 0.0f;
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN))
	{	// 後ろ
		g_Player.speed = VALUE_MOVE;
		g_Player.direction = XM_PI;
	}


	// 移動処理
	// XZ平面移動
	// プレイヤーの向いている方向をカメラからオフセット
	rotation.y = g_Player.direction + camRotation.y;

	// 入力のあった方向へプレイヤーを向かせて移動させる
	// プレイヤーの向いている方向を保存
	float deltaX = sinf(rotation.y) * g_Player.speed * deltaTime;
	float deltaZ = cosf(rotation.y) * g_Player.speed * deltaTime;
	g_Player.speed *= 0.2f;
	//移動を反映
	position.x += deltaX;
	position.z += deltaZ;

	// 高度（Y軸移動）
	float deltaY = 0.0f * deltaTime;
	position.y += deltaY;

	// 移動を反映(これ以上プレイヤーの位置を変えない)
	g_Player.object.SetPosition(position);
	g_Player.object.SetRotation(rotation);

	// 弾発射処理
	if (GetKeyboardTrigger(DIK_SPACE))
	{
 		SetBullet(position, camRotation);
	}
	PrintDebugProc((char*)"Player Information\nMovement:   W\n            A  S  D\n  Shift    Space\nPosition:(%f, %f, %f)\nRotation:(%f, %f, %f)\n", position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);

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