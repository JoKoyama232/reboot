//=============================================================================
//
// ���U���g�e�N�X�`������ [title.h]
// Author : ��c�@�P
//
//=============================================================================
#pragma once
#include "WorldSpaceClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class BUTTON : public WorldSpace
{
public:
	XMFLOAT3	pos;		//�|���S���̍��W
	float		w, h;		//���ƍ���
	float		alpha;		//�����x
	int			texNo;		//�g�p���Ă���e�N�X�`���ԍ�
	bool		flag_alpha;
	bool		flag_sound;
	bool		use;

};




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResultTex(void);
void UninitResultTex(void);
void UpdateResultTex(void);
void DrawResultTex(void);

