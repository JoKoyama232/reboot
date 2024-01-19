//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit(void);
BOOL CollisionBC(XMFLOAT3 pos1, XMFLOAT3 pos2, float r1, float r2);
BOOL CheckBase(XMFLOAT3 pos1, XMFLOAT3 scale1, XMFLOAT3 pos2, XMFLOAT3 scale2);
BOOL CheckPlayer(XMFLOAT3 ppos, XMFLOAT3 pscale1, XMFLOAT3 dpos, XMFLOAT3 dscale);
BOOL CheckDebris(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 dPos, XMFLOAT3 dScale);
BOOL CheckLX(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 xPos, XMFLOAT3 xScale);
BOOL CheckLY(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 yPos, XMFLOAT3 yScale);
BOOL CheckLZ(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 zPos, XMFLOAT3 zScale);

