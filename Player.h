// �v���C���[���� [player.h]
// Author : ���R�@��
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER		(1)					// �v���C���[�̐�
#define	PLAYER_SIZE		(5.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
	WorldSpace			object;
	
	float				speed;			// �ړ��X�s�[�h
	float				boost;			// �ړ��X�s�[�h

	float				direction;		// ����
	float				str;
	float				str_max;
	float				str_flag;
	float				size;
	float				Calpha;
	float				C2alpha;
	float				Aalpha;
	float				Ralpha;
	bool				use;

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�(���͎g��Ȃ����ȁH)
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER* GetPlayer(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
