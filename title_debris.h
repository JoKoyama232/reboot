//=============================================================================
// タイトルデブリ処理 [title_debris.h]
// Author : 岩田
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DEBRIS		(6)					// デブリの数
#define	DEBRIS_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TDEBRIS
{
	WorldSpace			object;

	float				speed;			// 移動スピード
	float				direction;		// 向き
	float				size;
	bool				use;			// 使用フラグ
	bool				flag_rotate;
	// 階層アニメーション用のメンバー変数(今は使わないかな？)
	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitleDebris(void);
void UninitTitleDebris(void);
void UpdateTitleDebris(void);
void DrawTitleDebris(void);
TDEBRIS* GetTitleDebris(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
