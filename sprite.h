//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11B132 99 外岡高明
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct SPRITE
{
	XMFLOAT2	pos;		// テクスチャの位置
	XMFLOAT2	size;		// テクスチャの大きさ
	XMFLOAT4	color;		// テクスチャの色

	bool		useColor;	// 色描画を使うフラグ
	bool		use;		// 描画可能フラグ
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetSprite(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteColor(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH, XMFLOAT4 color);

void SetSpriteColorRotation(ID3D11Buffer *buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot);

void SetSpriteLeftTop(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteLTColor(ID3D11Buffer* buf,
	float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color);
