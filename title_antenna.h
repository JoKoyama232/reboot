//=============================================================================
// タイトルアンテナ処理 [title_antenna.h]
// Author : 岩田　輝
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ANTENNA		(6)					// デブリの数
#define	ANTENNA_SIZE	(10.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct T_ANTENNA
{
	WorldSpace			object;

	float				speed;			// 移動スピード
	float				direction;		// 向き
	float				size;
	bool				use;			// 使用フラグ
	bool				flag_rotate;	// 回転フラグ
	// 階層アニメーション用のメンバー変数(今は使わないかな？)
	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitleAntenna(void);
void UninitTitleAntenna(void);
void UpdateTitleAntenna(void);
void DrawTitleAntenna(void);
T_ANTENNA* GetTitleAntenna(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
