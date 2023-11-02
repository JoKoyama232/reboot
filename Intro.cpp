//=============================================================================
// 初期画面処理 [Intro.cpp]
// Author : 小山　城
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"
#include "input.h"
#include "fade.h"

// 自身のインクルード	
#include "Intro.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(4)				// テクスチャの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
enum IntroTexture
{
	background = 0,
	introBackground,
	logo,
	credit,
	
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TextureName[TEXTURE_MAX] = { // テクスチャファイルアドレス配列
	(char*) "Data/texture/sunrise.jpg",
	(char*) "Data/texture/fade_black.png",
	(char*) "Data/texture/Logo.png",
	(char*) "Data/texture/myName.png",

};

static BOOL		g_Use;					// TRUE:使っている  FALSE:未使用
static float	g_w, g_h;				// 幅と高さ
static XMFLOAT3	g_Pos;					// ポリゴンの座標
static int		g_TexNo;				// テクスチャ番号
static SPRITE	sprite[TEXTURE_MAX];	// テクスチャ配列
static DWORD	timer;					// タイマー（時間制御のため）
static bool		g_Load = false;			// ロードフラグ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitIntro(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);

		sprite[i].pos = XMFLOAT2(0.0f, 0.0f);
		sprite[i].size = XMFLOAT2(0.0f, 0.0f);
		sprite[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		sprite[i].use = true;
		sprite[i].useColor = false;
	}

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_w = SCREEN_WIDTH;
	g_h = SCREEN_HEIGHT;

	sprite[introBackground].pos = { g_w / 2, g_h / 2 };
	sprite[introBackground].size = { (float)TEXTURE_WIDTH, (float)TEXTURE_HEIGHT };
	sprite[introBackground].useColor = true;
	sprite[introBackground].use = true;

	sprite[logo].pos = { 530.0f, g_h / 2 };
	sprite[logo].size = { 486.0f, 168.0f };
	sprite[logo].useColor = true;
	sprite[logo].color.w = 0.0f;
	sprite[logo].use = true;

	sprite[credit].pos = { g_w / 2, 520.0f };
	sprite[credit].size = { 210, 10 };
	sprite[credit].useColor = true;
	sprite[credit].color.w = 0.0f;
	sprite[credit].use = true;

	sprite[background].pos = { g_w / 2, g_h / 2 };
	sprite[background].size = { (float)TEXTURE_WIDTH, (float)TEXTURE_HEIGHT };

	timer = timeGetTime();
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitIntro(void)
{
	if (g_Load == false) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateIntro(void)
{
	DWORD time = timeGetTime() - timer;
	if (GetKeyboardTrigger(DIK_RETURN)) { time = 8001; }
	PrintDebugProc((char*)"Timer = %d\n", time);

	if (time > 8000)
	{
		sprite[logo].use = false;
		sprite[credit].use = false;
		sprite[introBackground].use = false;
		setLoad(false);
		// イントロが終わればタイトルへ移行
		SetMode(MODE_TITLE);
	}
	else if (time > 7000)
	{
		sprite[logo].color.w = 1 - (time - 7000) / 1000.0f;
		sprite[credit].color.w = 1 - (time - 7000) / 1000.0f;
		sprite[introBackground].color.w = 1 - (time - 7000) / 1000.0f;
		return;
	}
	else if (time > 3000)
	{
		sprite[credit].color.w = (time - 3000) / 1000.0f;
	}
	else if (time > 1000)
	{
		sprite[logo].color.w = (time - 1000) / 2000.0f;
	}


	



}

//=============================================================================
// 描画処理
//=============================================================================
void DrawIntro(void)
{
	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(false);

	// ライティングを無効
	SetLightEnable(false);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (!sprite[i].use)
			continue;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[i]);

		if (sprite[i].useColor) {
			SetSpriteColor(g_VertexBuffer, sprite[i].pos.x, sprite[i].pos.y, sprite[i].size.x, sprite[i].size.y, 0.0f, 0.0f, 1.0f, 1.0f, sprite[i].color);
		}
		else {
			SetSprite(g_VertexBuffer, sprite[i].pos.x, sprite[i].pos.y, sprite[i].size.x, sprite[i].size.y, 0.0f, 0.0f, 1.0f, 1.0f);
		}
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// ライティングを有効に
	SetLightEnable(true);

	// Z比較あり
	SetDepthEnable(true);
}










