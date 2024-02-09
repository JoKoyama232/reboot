//プレイヤーUI表示のcpp
//=============================================================================

#include "main.h"
#include "camera.h"
#include "playerUI.h"

#include "Player.h"
#include "bullet.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//****************************************************************************
#define TEXTURE_MAX			(10)								// テクスチャの数
#define PLAYER_UI_MAX		(1)								// プレイヤーのUIの数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
playerUI g_Hit;

static const char* g_TexturName[TEXTURE_MAX] = {
	"Data/texture/bar_white.png",
	"Data/texture/bullet.png",
	"Data/texture/capture.png",
	"Data/texture/attach.png",
	"Data/texture/collect.png",
	"Data/texture/meter_base.png",
	"Data/texture/reticle.png",
	"Data/texture/hud_2.png",
	"Data/texture/reroad.png",
	"Data/texture/hit.png",
};

HRESULT InitPlayerUI(void)
{
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

	return S_OK;
}

//=============================================================================
// // アタッチ表示処理
//=============================================================================
void DrawAttach(void)
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

	//ゲージの位置やテクスチャー座標を反映
	float px = 1000.0f;						// ゲージの表示位置X
	float py = 150.0f;						// ゲージの表示位置Y
	float pw = 400.0f;						// ゲージの表示幅
	float ph = 200.0f;						// ゲージの表示高さ

	float tw = 1.0f;						// テクスチャの幅
	float th = 1.0f;						// テクスチャの高さ
	float tx = 0.0f;						// テクスチャの左上X座標
	float ty = 0.0f;						// テクスチャの左上Y座標

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

	if (player->Aalpha >= 0.0f)
	{
		player->Aalpha -= 0.01;
	}


	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, player->Aalpha));


	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// // キャプチャー表示処理
//=============================================================================
void DrawCapture(void)
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

	//ゲージの位置やテクスチャー座標を反映
	float px = 1000.0f;						// ゲージの表示位置X
	float py = 150.0f;						// ゲージの表示位置Y
	float pw = 400.0f;						// ゲージの表示幅
	float ph = 200.0f;						// ゲージの表示高さ

	float tw = 1.0f;						// テクスチャの幅
	float th = 1.0f;						// テクスチャの高さ
	float tx = 0.0f;						// テクスチャの左上X座標
	float ty = 0.0f;						// テクスチャの左上Y座標

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

	if (player->Calpha >= 0.0f)
	{
		player->Calpha -= 0.01;
	}

	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, player->Calpha));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// コレクト表示処理
//=============================================================================
void DrawCollect(void)
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

	//ゲージの位置やテクスチャー座標を反映
	float px = 670.0f;						// ゲージの表示位置X
	float py = 150.0f;						// ゲージの表示位置Y
	float pw = 600.0f;						// ゲージの表示幅
	float ph = 200.0f;						// ゲージの表示高さ

	float tw = 1.0f;						// テクスチャの幅
	float th = 1.0f;						// テクスチャの高さ
	float tx = 0.0f;						// テクスチャの左上X座標
	float ty = 0.0f;						// テクスチャの左上Y座標

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

	if (player->C2alpha >= 0.0f)
	{
		player->C2alpha -= 0.01;
	}


	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, player->C2alpha));


	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// ヒット表示処理
//=============================================================================
void DrawHit(void)
{
	PLAYER* player = GetPlayer();
	BULLET* bullet = GetBullet();
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (bullet[i].flag_Halpha)
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

			//ゲージの位置やテクスチャー座標を反映
			float px = 670.0f;					// ゲージの表示位置X
			float py = 150.0f;						// ゲージの表示位置Y
			float pw = 600.0f;						// ゲージの表示幅
			float ph = 200.0f;						// ゲージの表示高さ

			float tw = 1.0f;						// テクスチャの幅
			float th = 1.0f;						// テクスチャの高さ
			float tx = 0.0f;						// テクスチャの左上X座標
			float ty = 0.0f;						// テクスチャの左上Y座標

			if (bullet[i].flag_Halpha == true)
			{
				bullet[i].Halpha -= 0.01f;
			}

			else if(bullet[i].flag_Halpha == false)
			{
				bullet[i].Halpha = 1.0f;
			}

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

			SetSpriteLTColor(g_VertexBuffer,
				px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, bullet[i].Halpha));


			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// リロード表示処理
//=============================================================================
void DrawReload(void)
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

	//ゲージの位置やテクスチャー座標を反映
	float reroad_x = 975.0f * 0.2;
	float reroad_y = 250.0f * 0.2;

	float px = SCREEN_CENTER_X - reroad_x/2;						// ゲージの表示位置X
	float py = SCREEN_CENTER_Y - reroad_y/2 - 200.0f;						// ゲージの表示位置Y
	float pw = reroad_x;						// ゲージの表示幅
	float ph = reroad_y;						// ゲージの表示高さ

	float tw = 1.0f;						// テクスチャの幅
	float th = 1.0f;						// テクスチャの高さ
	float tx = 0.0f;						// テクスチャの左上X座標
	float ty = 0.0f;						// テクスチャの左上Y座標

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, player->Ralpha));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

//=============================================================================
// // プレイヤーのUI表示処理
//=============================================================================
void DrawPlayerUI(void)
{
	for (int i = 0; i < PLAYER_UI_MAX; i++)
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

		float boost_x = 16.0f;
		float boost_y = 350.0f*0.6;

		float px = SCREEN_CENTER_X - boost_x / 2 - 200.0f;						// ゲージの表示位置X
		float py = SCREEN_CENTER_Y - boost_y / 2 + 100.0f;						// ゲージの表示位置Y
		float pw = boost_x;						// ゲージの表示幅
		float ph = boost_y;						// ゲージの表示高さ

		float tw = 50.0f;						// テクスチャの幅
		float th = 1.0f;						// テクスチャの高さ
		float tx = 0.0f;						// テクスチャの左上X座標
		float ty = 0.0f;						// テクスチャの左上Y座標

		PLAYER* player = GetPlayer();

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		SetSpriteLTColor(g_VertexBuffer,
			px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// プレイヤーのHPに従ってゲージの長さを表示
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		//ゲージの位置やテクスチャー座標を反映
		{
			ph = ph * (player->str / player->str_max);
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 0.0f, 0.5f));

		}
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}
//=============================================================================
// // プレイヤーの残弾表示処理
//=============================================================================
void DrawPlayerRestBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		BULLET* bullet = GetBullet();
		if (bullet[i].object.draw)continue;
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

			float bullet_x = 30.0f;
			float bullet_y = 30.0f;

			float px = SCREEN_CENTER_X - bullet_x / 2 + 200.0f;						// ゲージの表示位置X
			float py = SCREEN_CENTER_Y - bullet_y / 2 - 100.0f + (37.5f * i);						// ゲージの表示位置Y
			float pw = bullet_x;						// ゲージの表示幅
			float ph = bullet_y;						// ゲージの表示高さ


			////ゲージの位置やテクスチャー座標を反映
			//float px = 70.0f;		// ゲージの表示位置X
			//float py = 125.0f + (37.5f * i);		// ゲージの表示位置Y
			//float pw = 30.0f;		// ゲージの表示幅
			//float ph = 30.0f;		// ゲージの表示高さ

			float tw = 1.0f;	// テクスチャの幅
			float th = 1.0f;	// テクスチャの高さ
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;	// テクスチャの左上Y座標

			// プレイヤーのHPに従ってゲージの長さを表示
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			//ゲージの位置やテクスチャー座標を反映
			{
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteLTColor(g_VertexBuffer,
					px, py, pw, ph,
					tx, ty, tw, th,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

			}
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// // センターレティクル表示処理
//=============================================================================
void DrawReticle(void)
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

	float reticle_size = 800.0f * 0.4;			//	レティクルのサイズ

	//ゲージの位置やテクスチャー座標を反映
	float px = SCREEN_CENTER_X - reticle_size / 2;				// ゲージの表示位置X
	float py = SCREEN_CENTER_Y - reticle_size / 2;				// ゲージの表示位置Y
	float pw = reticle_size;						// ゲージの表示幅
	float ph = reticle_size;						// ゲージの表示高さ

	float tw = 1.0f;						// テクスチャの幅
	float th = 1.0f;						// テクスチャの高さ
	float tx = 0.0f;						// テクスチャの左上X座標
	float ty = 0.0f;						// テクスチャの左上Y座標

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);

	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, player->Realpha));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// // パラメータ表示処理
//=============================================================================
void DrawMeterBase(void)
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

	float meter_size = 1275.0f * 0.4;			//	レティクルのサイズ

	//ゲージの位置やテクスチャー座標を反映
	float px = SCREEN_CENTER_X - meter_size / 2;				// ゲージの表示位置X
	float py = SCREEN_CENTER_Y - meter_size / 2;				// ゲージの表示位置Y
	float pw = meter_size;						// ゲージの表示幅
	float ph = meter_size;						// ゲージの表示高さ

	float tw = 1.0f;						// テクスチャの幅
	float th = 1.0f;						// テクスチャの高さ
	float tx = 0.0f;						// テクスチャの左上X座標
	float ty = 0.0f;						// テクスチャの左上Y座標

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}


playerUI* GetHit(void)
{
	return &g_Hit;
}