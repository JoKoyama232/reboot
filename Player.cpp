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
#include "sprite.h"
#include "sound.h"
#include "iostream"
#include "random"
//*****************************************************************************
// マクロ定義
//****************************************************************************
#define	MODEL_PLAYER			"Data/model/robo_low.obj"		// 読み込むモデル名(まだ存在してないよ)

#define	VALUE_MOVE			(3.0f)							// 移動量

#define PLAYER_UI_MAX		(1)								// プレイヤーのUIの数
#define TEXTURE_MAX			(5)								// テクスチャの数

static float		roty = 0.0f;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static float        g_LastUpdate;
PLAYER g_Player;
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static const char* g_TexturName[TEXTURE_MAX] = {
	"Data/texture/bar_white.png",
	"Data/texture/ball_white.png",
	"Data/texture/capture.png",
	"Data/texture/attach.png",
	"Data/texture/collect.png",

};

HRESULT InitPlayer(void) {
	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	LoadModel(MODEL_PLAYER, &g_Player.object.modelInfo);
	GetModelDiffuse(&g_Player.object.modelInfo, g_Player.object.modelDiffuse);
	g_Player.object.load = true;

	g_Player.object.SetPosition(XMFLOAT3{ 0.0f, 0.0f, -100.0f });
	g_Player.object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
	g_Player.object.SetScale(XMFLOAT3{ 1.0f, 1.0f, 1.0f });
	g_Player.str = 100.0f;
	g_Player.str_max = 100.0f;
	g_Player.Aalpha - 0.0f;
	g_Player.Calpha = 0.0f;
	g_Player.C2alpha = 0.0f;
	g_Player.Ralpha = 0.0f;
	g_Player.time = 0.0f;
	g_Player.speed = 0.0f;			// 移動スピードクリア
	g_Player.use = true;
	g_Player.flag_Aalpha = false;
	g_Player.size = PLAYER_SIZE;
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
	XMFLOAT3 camat = cam->at;
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
		if ((GetKeyboardPress(DIK_A)) || (IsButtonPressed(0, BUTTON_LEFT)))
		{
			g_Player.direction = -XM_PI * 1 / 4;
		}
		else if ((GetKeyboardPress(DIK_D)) || (IsButtonPressed(0, BUTTON_RIGHT)))
		{
			g_Player.direction = XM_PI * 1 / 4;
		}

	}
	else if (GetKeyboardPress(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN))
	{	// 後ろ
		g_Player.speed = VALUE_MOVE;
		g_Player.direction = XM_PI;
		if ((GetKeyboardPress(DIK_A)) || (IsButtonPressed(0, BUTTON_LEFT)))
		{
			g_Player.direction = -XM_PI * 3 / 4;
		}
		else if ((GetKeyboardPress(DIK_D)) || (IsButtonPressed(0, BUTTON_RIGHT)))
		{
			g_Player.direction = XM_PI * 3 / 4;
		}

	}

	if (GetKeyboardPress(DIK_Q))
	{
		position.y += 1.0f;
		g_Player.str -= 0.3f;
	}

	else if (GetKeyboardPress(DIK_Z))
	{
		position.y -= 1.0f;
	}

	

	//スタミナゲージ処理&移動中のSE処理
	if (g_Player.speed == VALUE_MOVE)
	{
		g_Player.str -= 0.3f;
		if (g_Player.str <= 0)
		{
			g_Player.str = 0;
			//g_Player.speed = 0.1f;
		}
		//PlaySound(SOUND_LABEL_SE_?????); //ブースター噴射音
	}
	else
	{
		g_Player.str += 0.25f;
		if (g_Player.str > g_Player.str_max)
		{
			g_Player.str = g_Player.str_max;
		}
	}

	// 移動処理
	// XZ平面移動
	// プレイヤーの向いている方向をカメラからオフセット
	rotation.y = g_Player.direction + camRotation.y;
	// 入力のあった方向へプレイヤーを向かせて移動させる
	// プレイヤーの向いている方向を保存
	float deltaX = sinf(rotation.y) * g_Player.speed;
	float deltaZ = cosf(rotation.y) * g_Player.speed;
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

	



	if ((GetKeyboardTrigger(DIK_SPACE)) || IsMouseLeftTriggered())
	{
		SetBullet(position, camRotation);
		PlaySound(SOUND_LABEL_SE_BULLET); //モチ発射音
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

	// モデル描画
	DrawModel(&g_Player.object.modelInfo);

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

