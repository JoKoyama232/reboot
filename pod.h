//=============================================================================
// �f�u������ [pod.h]
// Author : ����l�@��
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_POD		(5)					// �f�u���̐�
#define	POD_SIZE		(10.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct POD
{
	WorldSpace			object;

	float				speed;			// �ړ��X�s�[�h
	float				direction;		// ����
	float				size;
	bool				use;			// �g�p�t���O
	bool				flag_rotate;
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�(���͎g��Ȃ����ȁH)
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPod(void);
void UninitPod(void);
void UpdatePod(void);
void DrawPod(void);
POD* GetPod(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);

