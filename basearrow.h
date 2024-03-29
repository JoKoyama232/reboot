//=============================================================================
// 拠点に向く矢の処理 [basearrow.h]
// Author : 岩田輝
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ARROW		(1)					// デブリの数
#define	ARROW_SIZE		(10.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ARROW
{
	WorldSpace			object;

	XMFLOAT3			at;				// カメラの注視点

	float				direction;		// 向き
	float				size;
	float				len;			// カメラの視点と注視点の距離
	bool				use;
	// 階層アニメーション用のメンバー変数(今は使わないかな？)
	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitArrow(void);
void UninitArrow(void);
void UpdateArrow(void);
void DrawArrow(void);
ARROW* GetArrow(void);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);

