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
struct Player
{
	WorldSpace			object;
	
	float				spd;		// �ړ��X�s�[�h
	float				dir;		// ����
	bool				use;

	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)


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

Player* GetPlayer(void);
XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage);
float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2);
