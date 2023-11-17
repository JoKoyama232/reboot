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
static RECT				g_windowPos;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	InitPlayer();

	InitDebris();

	//�e(���`)�̏�����
	InitBullet();

	//���v�̏�����
	InitClock();

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

	UninitSkybox();

	//�e(���`)�̏I������
	UninitBullet();

	// ���v�̏I������
	UninitClock();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	UpdatePlayer();

	UpdateDebris();

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
	XMFLOAT3 p_pos, d_pos, b_pos;
	PLAYER* player = GetPlayer();
	DEBRIS* debris = GetDebris();
	BULLET* bullet = GetBullet();
	p_pos = GetPlayer()->object.GetPositionFloat();

	for (int b = 0; b < MAX_BULLET; b++)
	{
		for (int d = 0; d < MAX_DEBRIS; d++)
		{
			//�e�ۂƃf�u���̍��W���Q�b�g
			b_pos = bullet[b].object.GetPositionFloat();
			d_pos = debris[d].object.GetPositionFloat();

			//�v���C���[�ƃf�u��
			if ((player->use == false) || (debris[d].use == false))
				continue;
			if (CollisionBC(p_pos, d_pos, player->size, debris[d].size))
			{
				debris[d].use = false;
				bullet[b].use = false;
				bullet[b].object.SetParent(NULL);
				bullet[b].spd = 1.0f;
				//�G�t�F�N�g�̃C���[�W�͋z�����܂�銴��(�}�C�N���̌o���l���߂�)

			}

			//���`�ƃf�u��
			if ((bullet[b].use == false) || (debris[d].use == false))
				continue;
			if (CollisionBC(b_pos, d_pos, bullet[b].size, debris[d].size))
			{
				bullet[b].spd = 0.0f;
				bullet[b].object.SetParent(&debris[d].object);
				
			}
		}
	}
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
