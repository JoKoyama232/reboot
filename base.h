//=============================================================================
// ������_���� [base.h]
// Author : ��c�P
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BASE		(1)					// �f�u���̐�
#define	BASE_SIZE		(30.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct BASE
{
	WorldSpace			object;

	float				speed;			// �ړ��X�s�[�h
	float				direction;		// ����
	float				size;
	bool				use;
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�(���͎g��Ȃ����ȁH)
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBase(void);
void UninitBase(void);
void UpdateBase(void);
void DrawBase(void);
BASE* GetBase(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);

