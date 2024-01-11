//=============================================================================
// UI表示処理 [UI.h]
// Author : 岩田輝
//=============================================================================
#include "main.h"
#include "sprite.h"
#include "renderer.h"


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

		//ゲージの位置やテクスチャー座標を反映
		float px = 40.0f;						// ゲージの表示位置X
		float py = 125.0f + (20.0f * i);		// ゲージの表示位置Y
		float pw = 8.0f;						// ゲージの表示幅
		float ph = 350.0f;						// ゲージの表示高さ

		float tw = 50.0f;						// テクスチャの幅
		float th = 1.0f;						// テクスチャの高さ
		float tx = 0.0f;						// テクスチャの左上X座標
		float ty = 0.0f;						// テクスチャの左上Y座標

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
			ph = ph * (g_Player.str / g_Player.str_max);
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

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

			//ゲージの位置やテクスチャー座標を反映
			float px = 70.0f;		// ゲージの表示位置X
			float py = 125.0f + (37.5f * i);		// ゲージの表示位置Y
			float pw = 30.0f;		// ゲージの表示幅
			float ph = 30.0f;		// ゲージの表示高さ

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
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			}
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}
