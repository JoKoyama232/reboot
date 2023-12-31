//=============================================================================
//
// スプライト処理 [sprite.cpp]
// Author : GP11B132 99 外岡高明
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************



//=============================================================================
// 頂点データ設定
//=============================================================================
void SetSprite(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// コンピューターは割り算が苦手
	hh = Height * 0.5f;		// 掛け算の方が処理が速い

	// 指定された座標を中心に設定するプログラム

	// 頂点０番（左上の頂点）
	vertex[0].Position = XMFLOAT3(X - hw, Y - hh, 0.0f);
	vertex[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(U, V);

	// 頂点１番（右上の頂点）
	vertex[1].Position = XMFLOAT3(X + hw, Y - hh, 0.0f);
	vertex[1].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	// 頂点２番（左下の頂点）
	vertex[2].Position = XMFLOAT3(X - hw, Y + hh, 0.0f);
	vertex[2].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	// 頂点３番（右下の頂点）
	vertex[3].Position = XMFLOAT3(X + hw, Y + hh, 0.0f);
	vertex[3].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	// 左上を原点として設定するプログラム
	//vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	//vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[0].TexCoord = D3DXVECTOR2(U, V);

	//vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	//vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);

	//vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	//vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);

	//vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	//vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);



	GetDeviceContext()->Unmap(buf, 0);

}


void SetSpriteLeftTop(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	// 左上を原点として設定するプログラム
	vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	vertex[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(U, V);

	vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	vertex[1].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	vertex[2].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	vertex[3].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);



	GetDeviceContext()->Unmap(buf, 0);

}

// 指定座標を左上として描画する
void SetSpriteLTColor(ID3D11Buffer* buf,
	float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 左上を原点として設定するプログラム
	vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(U, V);

	vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}


void SetSpriteColor(ID3D11Buffer *buf, float X, float Y, float Width, float Height,
		float U, float V, float UW, float VH,
		XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// コンピューターは割り算が苦手
	hh = Height * 0.5f;		// 掛け算の方が処理が速い

	// 指定された座標を中心に設定するプログラム

	// 頂点０番（左上の頂点）
	vertex[0].Position = XMFLOAT3(X - hw, Y - hh, 0.0f);
	vertex[0].Diffuse  = color;
	vertex[0].TexCoord = XMFLOAT2(U, V);

	// 頂点１番（右上の頂点）
	vertex[1].Position = XMFLOAT3(X + hw, Y - hh, 0.0f);
	vertex[1].Diffuse  = color;
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	// 頂点２番（左下の頂点）
	vertex[2].Position = XMFLOAT3(X - hw, Y + hh, 0.0f);
	vertex[2].Diffuse  = color;
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	// 頂点３番（右下の頂点）
	vertex[3].Position = XMFLOAT3(X + hw, Y + hh, 0.0f);
	vertex[3].Diffuse  = color;
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}

int SetSpriteGlitch(ID3D11Buffer* buf, float positionX, float positionY, float width, float height,
	int glitchRow, int totalRows, float glitchOffset, XMFLOAT4 glitchColor)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	XMFLOAT3 topLeft = XMFLOAT3(positionX - width * 0.5f, positionY - height * 0.5f, 0.0f);
	XMFLOAT4 defaultColor = XMFLOAT4(1.0f, 1.0f, 1.0f, glitchColor.w);
	int idx = 0;
	float cutoff = (float)(glitchRow % totalRows) / totalRows;

	// 変更が一番上の場合描画する”四角”は２つになるためインデックスを可変にする必要がある
	// グリッチの上のテクスチャ設定
	if (cutoff != 0) {
		// 頂点０番（左上の頂点）
		vertex[idx].Position = topLeft;
		vertex[idx].Diffuse = defaultColor;
		vertex[idx].TexCoord = XMFLOAT2(0.0f, 0.0f);
		idx++;

		// 頂点１番（右上の頂点）
		vertex[idx].Position = XMFLOAT3(topLeft.x + width, topLeft.y, 0.0f);
		vertex[idx].Diffuse = defaultColor;
		vertex[idx].TexCoord = XMFLOAT2(1.0f, 0.0f);
		idx++;

		// 頂点２番（左下の頂点）
		vertex[idx].Position = XMFLOAT3(topLeft.x, topLeft.y + height * cutoff, 0.0f);
		vertex[idx].Diffuse = defaultColor;
		vertex[idx].TexCoord = XMFLOAT2(0.0f, cutoff);
		idx++;

		// 頂点３番（右下の頂点）(四角の最後の為二回入力)
		vertex[idx].Position = XMFLOAT3(topLeft.x + width, topLeft.y + height * cutoff, 0.0f);
		vertex[idx].Diffuse = defaultColor;
		vertex[idx].TexCoord = XMFLOAT2(1.0f, cutoff);
		idx++;
		
	}

	// グリッチ描画
	// 頂点０番（左上の頂点）
	vertex[idx].Position = XMFLOAT3(topLeft.x + glitchOffset, topLeft.y + height * cutoff, 0.0f);;
	vertex[idx].Diffuse = glitchColor;
	vertex[idx].TexCoord = XMFLOAT2(0.0f, cutoff);
	idx++;

	// 頂点１番（右上の頂点）
	vertex[idx].Position = XMFLOAT3(topLeft.x + width + glitchOffset, topLeft.y + height * cutoff, 0.0f);
	vertex[idx].Diffuse = glitchColor;
	vertex[idx].TexCoord = XMFLOAT2(1.0f, cutoff);
	idx++;

	// 分割を１マス分下げる
	cutoff = (float)((glitchRow + 1) % totalRows) / totalRows;

	// 頂点２番（左下の頂点）
	vertex[idx].Position = XMFLOAT3(topLeft.x + glitchOffset, topLeft.y + height * cutoff, 0.0f);
	vertex[idx].Diffuse = glitchColor;
	vertex[idx].TexCoord = XMFLOAT2(0.0f, cutoff);
	idx++;

	// 頂点３番（右下の頂点）(テクスチャの最後の為一回で十分、下にあれば同じデータ追加）

	vertex[idx].Position = XMFLOAT3(topLeft.x + width + glitchOffset, topLeft.y + height * cutoff, 0.0f);
	vertex[idx].Diffuse = glitchColor;
	vertex[idx].TexCoord = XMFLOAT2(1.0f, cutoff);
	idx++;
	

	// 変更が一番下の場合描画する”四角”は２つになるためインデックスを可変にする必要がある
	// グリッチの上のテクスチャ設定
	if (cutoff < 1.0f) {
		// 頂点０番（左上の頂点）
		vertex[idx].Position = XMFLOAT3(topLeft.x, topLeft.y + height * cutoff, 0.0f);;
		vertex[idx].Diffuse = defaultColor;
		vertex[idx].TexCoord = XMFLOAT2(0.0f, cutoff);
		idx++;

		// 頂点１番（右上の頂点）
		vertex[idx].Position = XMFLOAT3(topLeft.x + width, topLeft.y + height * cutoff, 0.0f);
		vertex[idx].Diffuse = defaultColor;
		vertex[idx].TexCoord = XMFLOAT2(1.0f, cutoff);
		idx++;

		// 頂点２番（左下の頂点）
		vertex[idx].Position = XMFLOAT3(topLeft.x, topLeft.y + height, 0.0f);
		vertex[idx].Diffuse = defaultColor;
		vertex[idx].TexCoord = XMFLOAT2(0.0f, 1.0f);
		idx++;

		// 頂点３番（右下の頂点）(テクスチャの最後の為一回で十分)
		vertex[idx].Position = XMFLOAT3(topLeft.x + width, topLeft.y + height, 0.0f);
		vertex[idx].Diffuse = defaultColor;
		vertex[idx].TexCoord = XMFLOAT2(1.0f, 1.0f);
	 idx++;
	}

	GetDeviceContext()->Unmap(buf, 0);
	return idx;
}


void SetSpriteColorRotation(ID3D11Buffer *buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// コンピューターは割り算が苦手
	hh = Height * 0.5f;		// 掛け算の方が処理が速い

	// 指定された座標を中心に回転するプログラム
	float BaseAngle = atan2f(hh, hw);			// 中心点から頂点に対する角度
	XMVECTOR temp = { hw, hh, 0.0f, 0.0f };
	temp = XMVector2Length(temp);				// 中心点から頂点に対する距離
	float Radius = 0.0f;
	XMStoreFloat(&Radius, temp);

	// ここでアフィン変換（sincosのやつ）を使って4頂点を回転させる
	float x = X - cosf(BaseAngle + Rot) * Radius;
	float y = Y - sinf(BaseAngle + Rot) * Radius;
	vertex[0].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(BaseAngle - Rot) * Radius;
	y = Y - sinf(BaseAngle - Rot) * Radius;
	vertex[1].Position = XMFLOAT3(x, y, 0.0f);

	x = X - cosf(BaseAngle - Rot) * Radius;
	y = Y + sinf(BaseAngle - Rot) * Radius;
	vertex[2].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(BaseAngle + Rot) * Radius;
	y = Y + sinf(BaseAngle + Rot) * Radius;
	vertex[3].Position = XMFLOAT3(x, y, 0.0f);

	vertex[0].Diffuse = Color;
	vertex[1].Diffuse = Color;
	vertex[2].Diffuse = Color;
	vertex[3].Diffuse = Color;

	vertex[0].TexCoord = XMFLOAT2(U, V);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}

