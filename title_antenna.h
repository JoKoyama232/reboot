//=============================================================================
// �^�C�g���A���e�i���� [title_antenna.h]
// Author : ��c�@�P
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ANTENNA		(6)					// �f�u���̐�
#define	ANTENNA_SIZE	(10.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct T_ANTENNA
{
	WorldSpace			object;

	float				speed;			// �ړ��X�s�[�h
	float				direction;		// ����
	float				size;
	bool				use;			// �g�p�t���O
	bool				flag_rotate;	// ��]�t���O
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�(���͎g��Ȃ����ȁH)
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitleAntenna(void);
void UninitTitleAntenna(void);
void UpdateTitleAntenna(void);
void DrawTitleAntenna(void);
T_ANTENNA* GetTitleAntenna(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
