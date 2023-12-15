//=============================================================================
// デバッグツール [DebugObj.h]
// Author : Jo Koyama
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	DEBUGOBJ_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct DebugObj
{
	WorldSpace	transform;
	bool		use = false;
	float		scale;
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebugObj(void);
void UninitDebugObj(void);
void UpdateDebugObj(void);
void DrawDebugObj(void);

int SetDebugObj(XMFLOAT3 position, XMFLOAT3 rotation, float size);
DebugObj* GetDebugObj(int idx);