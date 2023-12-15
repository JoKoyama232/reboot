//=============================================================================
// デブリ処理 [rocket.h]
// Author : 小井詰　礼
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ROCKET		(5)					// デブリの数
#define	ROCKET_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ROCKET
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
HRESULT InitRocket(void);
void UninitRocket(void);
void UpdateRocket(void);
void DrawRocket(void);
ROCKET* GetRocket(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
