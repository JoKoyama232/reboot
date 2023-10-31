//=============================================================================
//
// タイトルテクスチャ処理 [title_tex.cpp]
// Author : 岩田　輝
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "title_tex.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 

#define TEXTURE_MAX					(5)				// テクスチャの数
#define BUTTON_MAX					(3)


#define TEXTURE_WIDTH_LOGO			(240)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(40)			// 

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報


static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/title_logo.png",
	"data/TEXTURE/game_start.png",
	"data/TEXTURE/exit_2.png",
	"data/TEXTURE/option.png",
};


static bool						g_Use;						// true:使っている  false:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

float	alpha;
bool	flag_alpha;

static BOOL						g_Load = FALSE;
static BUTTON					g_Button[BUTTON_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitleTex(void)
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


	// 変数の初期化
	g_Use = true;
	g_h = TEXTURE_HEIGHT;
	g_w = TEXTURE_WIDTH;
	g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = true;

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		g_Button[i].use = false;
		g_Button[i].texNo = 1 + i;
		g_Button[i].h = TEXTURE_HEIGHT;
		g_Button[i].w = TEXTURE_WIDTH;
		g_Button[i].pos = XMFLOAT3(g_Button[i].w / 2, g_Button[i].h / 2 + 100 + (50.0f * i), 0.0f);

		g_Button[i].alpha = 1.0f;
		g_Button[i].flag_alpha = true;
		g_Button[i].flag_sound = true;

	}


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitleTex(void)
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
void UpdateTitleTex(void)
{
	int x = GetMousePosX();
	int y = GetMousePosY();

	for (int i{}; i < BUTTON_MAX; i++)
	{
		HWND windowHandle = GetForegroundWindow(); // ウィンドウのハンドルを取得
		if (windowHandle != NULL)
		{
			RECT windowRect;
			if (GetWindowRect(windowHandle, &windowRect)) // ウィンドウの位置情報を取得
			{
				// マウルの位置が画像に当たっているかどうかの判定
				if ((x > g_Button[i].pos.x - TEXTURE_WIDTH_LOGO / 2) &&
					(x < g_Button[i].pos.x + TEXTURE_WIDTH_LOGO / 2) &&
					(y > g_Button[i].pos.y - TEXTURE_HEIGHT_LOGO / 2) &&
					(y < g_Button[i].pos.y + TEXTURE_HEIGHT_LOGO / 2))
				{
					//点滅させる
					if (g_Button[i].flag_alpha == true)
					{
						g_Button[i].alpha -= 0.02f;
						if (g_Button[i].alpha <= 0.0f)
						{
							g_Button[i].alpha = 0.0f;
							g_Button[i].flag_alpha = false;
						}
					}
					else
					{
						g_Button[i].alpha += 0.02f;
						if (g_Button[i].alpha >= 1.0f)
						{
							g_Button[i].alpha = 1.0f;
							g_Button[i].flag_alpha = true;
						}
					}

					if (g_Button[i].flag_sound == true)
					{
						PlaySound(SOUND_LABEL_SE_ZIPPO);
						g_Button[i].flag_sound = false;
					}

					//マウスの左ボタンが押されたら
					if (GetKeyState(VK_LBUTTON) & 0x80)
					{
						if (i == 0)
						{
							SetFade(FADE_OUT, MODE_GAME);
							//PlaySound();
						}
						else if (i == 1)
						{
							int id = MessageBox(NULL, "ゲームを終了しますか？", "", MB_YESNO | MB_ICONQUESTION);
							switch (id)
							{
							case IDYES:		// ゲームを終了
								exit (-1);
								break;
							case IDNO:		// 何もせずにタイトルに戻る
								
								break;
							}
						}
						else if (i == 2)
						{

						}
					}




				}
				else //マウスが画像の範囲外なら点滅せずに表示
				{
					g_Button[i].alpha = 1.0f;
					g_Button[i].flag_sound = true;
				}

				
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitleTex(void)
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
		//タイトルのロゴを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
		//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
			SetSpriteColor(g_VertexBuffer, g_Pos.x, g_Pos.y - 50.0f,
				TEXTURE_WIDTH_LOGO * 2.5f, TEXTURE_HEIGHT_LOGO * 2.5f,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		//ボタンを描画
		for (int i = 0; i < BUTTON_MAX; i++)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Button[i].texNo]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, g_Button[i].pos.x, g_Button[i].pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO,
				0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, g_Button[i].alpha));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}


}