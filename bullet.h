//=============================================================================
// 弾丸処理 [bullet.h]
// Author :
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BULLET		(10)				// 弾丸(モチ)の数(仮)
#define	BULLET_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct BULLET
{
	WorldSpace			object;

	float				spd;		// 移動スピード
	float				dir;		// 向き
	float				size;
	float				Halpha;
	bool				flag_Halpha;
	bool				use;		// 使用フラグ

	// 階層アニメーション用のメンバー変数
	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);

BULLET* GetBullet(void);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
