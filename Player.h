// プレイヤー処理 [player.h]
// Author : 小山　城
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER		(1)					// プレイヤーの数
#define	PLAYER_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct Player
{
	WorldSpace			object;
	
	float				spd;		// 移動スピード
	float				dir;		// 向き
	bool				use;

	// 階層アニメーション用のメンバー変数(今は使わないかな？)
	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

Player* GetPlayer(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
