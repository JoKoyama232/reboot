//�v���C���[��UI�\��
#pragma once

struct playerUI
{
	bool				hflag;

};

//�v���g�^�C�v�錾
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