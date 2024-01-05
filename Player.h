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
struct PLAYER
{
	WorldSpace			object;
	
	float				speed;			// 移動スピード
	float				direction;		// 向き
	float				str;
	float				str_max;
	float				str_flag;
	float				size;
	float				Calpha;
	float				C2alpha;
	float				Aalpha;
	bool				use;
	bool				flag_alpha;
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
void DrawAttach(void);
void DrawCapture(void);
void DrawCollect(void);
void DrawPlayerUI(void);
void DrawPlayerRestBullet(void);
PLAYER* GetPlayer(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
