//=============================================================================
// �e�ۏ��� [bullet.h]
// Author :
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BULLET		(10)				// �e��(���`)�̐�(��)
#define	BULLET_SIZE		(5.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct BULLET
{
	WorldSpace			object;

	float				spd;		// �ړ��X�s�[�h
	float				dir;		// ����
	float				size;
	float				Halpha;
	bool				flag_Halpha;
	bool				use;		// �g�p�t���O

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);

BULLET* GetBullet(void);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
