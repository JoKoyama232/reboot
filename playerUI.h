//�v���C���[��UI�\��
#pragma once

class playerUI
{
	float				Calpha;
	float				C2alpha;
	float				Aalpha;
	float				Ralpha;
	bool				flag_Aalpha;

};

//�v���g�^�C�v�錾
HRESULT InitPlayerUI(void);
void DrawAttach(void);
void DrawCapture(void);
void DrawCollect(void);
void DrawReload(void);
void DrawPlayerUI(void);
void DrawPlayerRestBullet(void);
void DrawReticle(void);
