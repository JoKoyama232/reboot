//=============================================================================
//
// リザルトテクスチャ画面処理 [result.cpp]
// Author : 岩田　輝
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "result_tex.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX					(6)				// テクスチャの数
#define BUTTON_MAX					(2)


#define TEXTURE_WIDTH_LOGO			(360)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(60)			// 

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/number.png",
	"data/TEXTURE/replay_mission.png",
	"data/TEXTURE/quit_game.png",
	"data/TEXTURE/mission_complete_result.png",
	"data/TEXTURE/result.png",
	"data/TEXTURE/mission_fail_result.png",


};


static bool						g_Use;						// true:使っている  false:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号
static int selector = 0;
static BOOL						g_Load = FALSE;
static BUTTON					g_Button[BUTTON_MAX];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResultTex(void)
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
	g_w = SCREEN_WIDTH;
	g_h = SCREEN_HEIGHT;
	g_Pos = { g_w * 0.5f, 50.0f, 0.0f };
	g_TexNo = 0;

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		g_Button[i].texNo = 1 + i;
		g_Button[i].h = SCREEN_HEIGHT;
		g_Button[i].w = SCREEN_WIDTH;
		g_Button[i].pos = XMFLOAT3(1600, g_Button[i].h * 0.5f + 150.0f * (i + 1), 0.0f);

		g_Button[i].alpha = 1.0f;
		g_Button[i].flag_alpha = true;
		g_Button[i].flag_sound = false;
	}

	//// BGM再生
	//PlaySound(SOUND_LABEL_BGM_sample002);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResultTex(void)
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
void UpdateResultTex(void)
{
	int x = GetMousePosX();
	int y = GetMousePosY();

	HWND windowHandle = GetForegroundWindow(); // ウィンドウのハンドルを取得
	RECT windowRect;
	GetWindowRect(windowHandle, &windowRect);// ウィンドウの位置情報を取得
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		// マウルの位置が画像に当たっているかどうかの判定
		if ((x > g_Button[i].pos.x - TEXTURE_WIDTH_LOGO * 0.5f) &&
			(x < g_Button[i].pos.x + TEXTURE_WIDTH_LOGO * 0.5f) &&
			(y + 25.0f > g_Button[i].pos.y - TEXTURE_HEIGHT_LOGO * 0.5f) &&
			(y + 30.0f < g_Button[i].pos.y + TEXTURE_HEIGHT_LOGO * 0.5f))
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
				switch (i) {
				case 0:
					SetFade(FADE_OUT, MODE_GAME);
					//マウス非表示
					ShowCursor(false);
					//PlaySound();
					break;
				case 1:
				{
					int id = MessageBox(NULL, "ゲームを終了しますか？", "", MB_YESNO | MB_ICONQUESTION);
					switch (id)
					{
					case IDYES:		// ゲームを終了
						exit(-1);
						break;
					case IDNO:		// 何もせずにタイトルに戻る

						break;
					}
				}
				break;
				}
			}

		}
		else //マウスが画像の範囲外なら点滅せずに表示
		{
			g_Button[i].alpha = 1.0f;
			g_Button[i].flag_sound = true;
		}

	}
	

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、ステージを切り替える
		SetFade(FADE_OUT, MODE_TITLE);
	}

	// ゲームパッドで入力処理
	if (IsButtonTriggered(0, BUTTON_START))
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}


#ifdef _DEBUG	// デバッグ情報を表示する

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResultTex(void)
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

	// result表の表示
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, 950.0f, 500.0f, 1059*0.8, 1074*0.8, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		GetDeviceContext()->Draw(4, 0);
	}

	// スコア表示
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// 桁数分処理する
		int number = GetScore();
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(number % 10);

			// スコアの位置やテクスチャー座標を反映
			float pw = 16 * 4;			// スコアの表示幅
			float ph = 32 * 4;			// スコアの表示高さ
			float px = 1250.0f - i * pw;	// スコアの表示位置X
			float py = 265.0f;			// スコアの表示位置Y

			float tw = 1.0f / 10;		// テクスチャの幅
			float th = 1.0f / 1;		// テクスチャの高さ
			float tx = x * tw;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			// 次の桁へ
			number /= 10;
		}

	}
	
	// ボーナスの表示
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// 桁数分処理する
		int bonus = GetBonus();
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(bonus % 10);

			// スコアの位置やテクスチャー座標を反映
			float pw = 16 * 4;			// スコアの表示幅
			float ph = 32 * 4;			// スコアの表示高さ
			float px = 1250.0f - i * pw;	// スコアの表示位置X
			float py = 450.0f;			// スコアの表示位置Y

			float tw = 1.0f / 10;		// テクスチャの幅
			float th = 1.0f / 1;		// テクスチャの高さ
			float tx = x * tw;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			// 次の桁へ
			bonus /= 10;
		}
	}

	// 合計スコアの表示
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// 桁数分処理する
		int number = GetScore();
		int bonus = GetBonus();
		int score = number + bonus;
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(score % 10);

			// スコアの位置やテクスチャー座標を反映
			float pw = 16 * 4;			// スコアの表示幅
			float ph = 32 * 4;			// スコアの表示高さ
			float px = 1250.0f - i * pw;	// スコアの表示位置X
			float py = 660.0f;			// スコアの表示位置Y

			float tw = 1.0f / 10;		// テクスチャの幅
			float th = 1.0f / 1;		// テクスチャの高さ
			float tx = x * tw;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			// 次の桁へ
			score /= 10;
		}
	}


	// congratulationsの表示
	{
		int score = GetScore();
		
		if (score < 1000)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, 950.0f, 850.0f, 889 * 0.5, 80 * 0.5, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}

		if (score >= 1000)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, 950.0f, 850.0f, 1279 * 0.5, 80 * 0.5, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}
	}

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




