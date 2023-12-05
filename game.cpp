//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"

#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "WorldSpaceClass.h"
#include "Skybox.h"
#include "clock.h"
#include "Player.h"
#include "debris.h"
#include "bullet.h"
#include "base.h"
#include "score.h"

#include "game.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

static bool	g_bPause = true;	// �|�[�YON/OFF
static int flag_score = 0;
static RECT	g_windowPos;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	InitPlayer();

	InitDebris();

	InitBase();

	//�e(���`)�̏�����
	InitBullet();

	//���v�̏�����
	InitClock();

	InitScore();

	InitSkybox();

	//�}�E�X��\��
	ShowCursor(false);



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	UninitPlayer();

	UninitDebris();

	UninitBase();

	UninitSkybox();

	//�e(���`)�̏I������
	UninitBullet();

	// ���v�̏I������
	UninitClock();

	UninitScore();

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	UpdatePlayer();

	UpdateDebris();

	UpdateBase();
	//�e(���`)�̍X�V����
	UpdateBullet();

	// ���v�̍X�V����
	UpdateClock();

	UpdateSkybox();

	//�����蔻��
	CheckHit();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	XMFLOAT3 pos;

	// �v���C���[���_
	pos = GetPlayer()->object.GetPositionFloat();
	pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
	SetCameraAT(pos);
	SetCamera();

	DrawSkybox();

	DrawPlayer();

	DrawDebris();

	DrawBase();
	//�e(���`)�̕`�揈��
	DrawBullet();

	// 2D�̕���`�悷�鏈��
	{
		// Z��r�Ȃ�
		SetDepthEnable(false);

		// ���C�e�B���O�𖳌�
		SetLightEnable(false);

		//�������牺��
		DrawClock();

		DrawScore();

		DrawPlayerUI();

		DrawPlayerRestBullet();
		// ���C�e�B���O��L����
		SetLightEnable(true);

		// Z��r����
		SetDepthEnable(true);
	}

}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	XMFLOAT3 p_pos, d_pos, b_pos, basepos;
	PLAYER* player = GetPlayer();			// �v���C���[���
	DEBRIS* debris = GetDebris();			// �f�u�����
	BULLET* bullet = GetBullet();			// �e�ۏ��
	BASE* base = GetBase();					// ��n���
	p_pos = GetPlayer()->object.GetPositionFloat();
	basepos = GetBase()->object.GetPositionFloat();

	// �v���C���[���g���Ă��Ȃ��̂Ȃ瓖���蔻����K�v�Ȃ����߂܂��v���C���[�̎g�p�t���O�m�F
	if (!player->use) return;

	// �e�ۖ��̓����蔻��----------------------------------------------------
	for (int b = 0; b < MAX_BULLET; b++)
	{
		// �e�ۂ̎g�p�t���O���m�F�A�܂��y�A�����g����Ă���Γ����蔻����s���Ȃ�
		if (!bullet[b].use || bullet[b].object.GetParent()) continue;

		// �o���b�g�΃f�u�������蔻��
		for (int d = 0; d < MAX_DEBRIS; d++)
		{
			// �f�u���̎g�p�t���O�m�F
			if (!debris[d].use) continue;
			
			// �e�ۂƃf�u���̍��W���Q�b�g
			b_pos = bullet[b].object.GetPositionFloat();
			d_pos = debris[d].object.GetPositionFloat();

			// �e�ۂƃf�u���̓����蔻��
			if (!CollisionBC(b_pos, d_pos, bullet[b].size, debris[d].size)) continue;
			
			// �e�ۂƃf�u���̓����蔽���i�^�j
			bullet[b].spd = 0.0f;
			debris[d].object.SetParent(&bullet[b].object);
		}
	}
	//----------------------------------------------------------------------

	// �v���C���[�̓����蔻��-------------------------------------------------

	bool unload = false;		// �ێ����Ă���f�u�������
	// �v���C���[�Ɗ�n�̓����蔻��
	if (CollisionBC(p_pos, basepos, player->size, base->size))
	{
		// �v���C���[�Ɗ�n�̓����蔽���i�^
		AddScore(flag_score * 100);
		flag_score = 0;
		
	}

	// �f�u�����ɔ���
	for (int d = 0; d < MAX_DEBRIS; d++)
	{
		// �f�u���̎g�p�t���O�m�F
		if (!debris[d].use) continue;
		d_pos = debris[d].object.GetPositionFloat();
		if (!CollisionBC(p_pos, d_pos, player->size, debris[d].size)) continue;
		
		// �v���C���[�ƃf�u���̓����蔽���i�^�j
		if (!debris[d].object.GetParent() == NULL)
		{
			PlaySound(SOUND_LABEL_SE_ABSORB);
			debris[d].use = false;
			flag_score += 1;
			for (int b = 0; b < MAX_BULLET; b++)
			{			
				// �e�ۂ̎g�p�t���O���m�F
				if (!bullet[b].use)continue;
				if (bullet[b].spd < 1.0f)
				{
					debris[d].object.SetParent(NULL);
					bullet[b].use = false;
					bullet[b].spd = 1.0f;
				}
			}
		}
		//�G�t�F�N�g�̃C���[�W�͋z�����܂�銴��(�}�C�N���̌o���l���߂�)
	}
	//----------------------------------------------------------------------

}




//=============================================================================
// BC�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���TRUE
//=============================================================================
BOOL CollisionBC(XMFLOAT3 pos1, XMFLOAT3 pos2, float r1, float r2)
{
	BOOL ans = FALSE;						// �O����Z�b�g���Ă���

	float len = (r1 + r2) * (r1 + r2);		// ���a��2�悵����
	XMVECTOR temp = XMLoadFloat3(&pos1) - XMLoadFloat3(&pos2);
	temp = XMVector3LengthSq(temp);			// 2�_�Ԃ̋����i2�悵�����j
	float lenSq = 0.0f;						// �^�ϊ��pfloat�^�ϐ��̐錾�Ə�����
	XMStoreFloat(&lenSq, temp);				// ��r�����̂��߂�float�^�Ɍ^�ϊ�

	// ���a��2�悵������苗�����Z���H
	if (len > lenSq)
	{
		ans = TRUE;	// �������Ă���
	}

	return ans;
}
