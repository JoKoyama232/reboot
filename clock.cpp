//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "clock.h"
#include "sprite.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(48)	// キャラサイズ
#define TEXTURE_HEIGHT				(96)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数
#define CLOCK_MAX			(6000)		// 制限時間(秒数)
#define CLOCK_DIGIT			(4)			// 桁数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/colon.png",
};


static bool						g_Use;						// true:使っている  false:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static float					g_Clock;					// スコア
static int						g_Count;				// カウント

static BOOL						g_Load = FALSE;
static bool						g_falg = false;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitClock(void)
{
	ID3D11Device* pDevice = GetDevice();

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


	// プレイヤーの初期化
	g_Use = true;
	g_w = TEXTURE_WIDTH + 10;
	g_h = TEXTURE_HEIGHT + 10;
	g_Pos = { 275.0f, 60.0f, 0.0f };
	g_TexNo = 0;
	g_falg = false;
	g_Clock = CLOCK_MAX;	// スコアの初期化

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitClock(void)
{
	if (g_Load == FALSE) return;

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
void UpdateClock(void)
{
	if (g_Clock <= 0.0f)	//6000フレーム(約1分)たったらゲームリザルトに行く
	{
		SetFade(FADE_OUT, MODE_RESULT);
		g_Clock = 0.0f;
	}

	if (g_Clock == 2500.0f)
	{
		g_falg = true;
		PlaySound(SOUND_LABEL_SE_ALARM);
		StopSound(SOUND_LABEL_BGM_stage1);
	}

	if (g_Clock == 2150.0f)
	{
		PlaySound(SOUND_LABEL_BGM_game2);
	}

	if (g_Clock >= 0.0f)
	{
		if (GetKeyboardTrigger(DIK_0))
		{
			SetFade(FADE_OUT, MODE_RESULT);
			ShowCursor(true);
		}
	}
	
#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawClock(void)
{
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

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	// 桁数分処理する
	int number = g_Clock;
	for (int i = 0; i < CLOCK_DIGIT; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(number);

		// スコアの位置やテクスチャー座標を反映
		float px = g_Pos.x - g_w * i;	// スコアの表示位置X
		float py = g_Pos.y;			// スコアの表示位置Y
		float pw = g_w;				// スコアの表示幅
		float ph = g_h;				// スコアの表示高さ

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = x * tw;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		if ((i == 2) || (i == 3))
		{
			px -= 40.0f;
		}

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		number /= 10;
	}

	// コロンの描画
	{
		//ゲージの位置やテクスチャー座標を反映
		float px = 165.0f;						// ゲージの表示位置X
		float py = 40.0f;						// ゲージの表示位置Y
		float pw = 13.0f;						// ゲージの表示幅
		float ph = 50.0f;						// ゲージの表示高さ

		float tw = 1.0f;						// テクスチャの幅
		float th = 1.0f;						// テクスチャの高さ
		float tx = 0.0f;						// テクスチャの左上X座標
		float ty = 0.0f;						// テクスチャの左上Y座標

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		SetSpriteLTColor(g_VertexBuffer,
			px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	
}


//=============================================================================
// 時間を経過させる
// 引数:add
//=============================================================================
void AddClock(int add)
{
	g_Clock -= add;
	if (g_Clock <= 0.0f)
	{
		g_Clock = 0.0f;
	}

}
