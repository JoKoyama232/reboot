//プレイヤーのUI表示
#pragma once

struct playerUI
{
	bool				hflag;

};

//プロトタイプ宣言
HRESULT InitPlayerUI(void);
void DrawAttach(void);
void DrawHit(void);
void DrawCapture(void);
void DrawCollect(void);
void DrawReload(void);
void DrawPlayerUI(void);
void DrawPlayerRestBullet(void);
void DrawReticle(void);
void DrawMeterBase(void);