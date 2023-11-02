
// デブリ処理 [debris.h]
// Author : 小井詰　礼
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DEBRIS		(1)					// デブリの数
#define	DEBRIS_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct DEBRIS
{
	WorldSpace			object;

	float				speed;			// 移動スピード
	float				direction;		// 向き
	float				str;
	float				str_max;
	float				str_flag;
	bool				use;
	// 階層アニメーション用のメンバー変数(今は使わないかな？)
	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebris(void);
void UninitDebris(void);
void UpdateDebris(void);
void DrawDebris(void);
DEBRIS* GetDebris(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
