//=============================================================================
// �f�u������ [satellite.h]
// Author : ����l�@��
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SATELLITE		(5)					// �f�u���̐�
#define	SATELLITE_SIZE		(5.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct SATELLITE
{
	WorldSpace			object;

	float				speed;			// �ړ��X�s�[�h
	float				direction;		// ����
	float				size;
	bool				use;			// �g�p�t���O

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�(���͎g��Ȃ����ȁH)
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSatellite(void);
void UninitSatellite(void);
void UpdateSatellite(void);
void DrawSatellite(void);
SATELLITE* GetSatellite(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
