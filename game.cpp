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
#include "antenna.h"
#include "solarpanel.h"
#include "hatch.h"
#include "pod.h"
#include "satellite.h"
#include "rocket.h"
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

	InitAntenna();

	InitPanel();

	InitHatch();

	InitPod();

	InitSatellite();

	InitRocket();

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

	UninitAntenna();

	UninitPanel();

	UninitHatch();

	UninitPod();

	UninitSatellite();

	UninitRocket();

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

	UpdateAntenna();

	UpdatePanel();

	UpdateHatch();

	UpdatePod();

	UpdateSatellite();

	UpdateRocket();

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
	SetCameraAT(pos);
	SetCamera();

	DrawSkybox();

	DrawPlayer();

	DrawDebris();

	DrawAntenna();

	DrawPanel();

	DrawHatch();

	DrawPod();

	DrawSatellite();

	DrawRocket();

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
	// �e�I�u�W�F�N�g�̈ʒu���`
	XMFLOAT3 podPos, basePos, hatchPos, panelPos, playerPos,
		debrisPos, bulletPos, rokeckPos, antennaPos, satellitePos;

	// �e�I�u�W�F�N�g�̏����擾
	POD* pod = GetPod();					// �|�b�h���
	BASE* base = GetBase();					// ��n���
	HATCH* hatch = GetHatch();				// �n�b�`���
	PANEL* panel = GetPanel();				// �\�[���[�p�l�����
	PLAYER* player = GetPlayer();			// �v���C���[���
	DEBRIS* debris = GetDebris();			// �f�u�����
	BULLET* bullet = GetBullet();			// �e�ۏ��
	ROCKET* rocket = GetRocket();			// ���P�b�g���
	ANTENNA* antenna = GetAntenna();		// �A���e�i���
	SATELLITE* satellite = GetSatellite();  // �T�e���C�g���
	
	// 1�����g��Ȃ��I�u�W�F�N�g�̈ʒu�����炩���ߒ�`
	playerPos = GetPlayer()->object.GetPositionFloat();
	basePos = GetBase()->object.GetPositionFloat();

	// �v���C���[���g���Ă��Ȃ��̂Ȃ瓖���蔻����K�v�Ȃ����߂܂��v���C���[�̎g�p�t���O�m�F
	if (!player->use) return;

	// �e�ۖ��̓����蔻��----------------------------------------------------
	for (int b = 0; b < MAX_BULLET; b++)
	{
		// �e�ۂ̎g�p�t���O���m�F
		if (!bullet[b].use) continue;
		bulletPos = bullet[b].object.GetPositionFloat();

		// �o���b�g�΃f�u�������蔻��
		for (int d = 0; d < MAX_DEBRIS; d++)
		{
			// �f�u���̎g�p�t���O�m�F
			if (!debris[d].use) continue;
			
			//�g�p�t���O���`�F�b�N��������W���Q�b�g
			debrisPos = debris[d].object.GetPositionFloat();

			// �e�ۂƃf�u���̓����蔻��
			if (!CollisionBC(bulletPos, debrisPos, bullet[b].size, debris[d].size)) continue;
			
			// �e�ۂƃf�u���̓����蔽���i�^�j
			bullet[b].spd = 0.0f;
			debris[d].flag_rotate = false;
			debris[d].object.SetParent(&bullet[b].object);
		}


		//�o���b�g�ƃA���e�i�̓����蔻��
		for (int a = 0; a < MAX_ANTENNA; a++)
		{
			//�A���e�i�̎g�p�t���O���`�F�b�N
			if (!antenna[a].use)continue;
			//�g�p�t���O���`�F�b�N��������W���Q�b�g
			antennaPos = antenna[a].object.GetPositionFloat();

			// �e�ۂƃA���e�i�̓����蔻��
			if (!CollisionBC(bulletPos, antennaPos, bullet[b].size, antenna[a].size))continue;
			
			bullet[b].spd = 0.0f;
			antenna[a].flag_rotate = false;
			antenna[a].object.SetParent(&bullet[b].object);
			
		}
	}
	//----------------------------------------------------------------------



	// �v���C���[�̓����蔻��-------------------------------------------------
	bool unload = false;		// �ێ����Ă���f�u�������
	// �v���C���[�Ɗ�n�̓����蔻��
	if (CollisionBC(playerPos, basePos, player->size, base->size))
	{
		// �v���C���[�Ɗ�n�̓����蔽���i�^
		if (!flag_score == 0)
		{
			PlaySound(SOUND_LABEL_SE_SCORE);
		}

		AddScore(flag_score * 100);
		flag_score = 0;
		for (int i = 0; i < MAX_BULLET; i++)
		{
			if (!bullet[i].spd == 1.0f)continue;
			{
				bullet[i].use = false;
			}
		}
	}

	// �f�u���Ƃ̏���
	for (int d = 0; d < MAX_DEBRIS; d++)
	{
		// �f�u���̎g�p�t���O�m�F
		if (!debris[d].use) continue;

		//�g�p�t���O���`�F�b�N��������W���Q�b�g
		debrisPos = debris[d].object.GetPositionFloat();
		
		// �v���C���[�ƃf�u���̓����蔻��
		if (!CollisionBC(playerPos, debrisPos, player->size, debris[d].size)) continue;
		
		// �v���C���[�ƃf�u���̓����蔽���i�^�j
		if (!debris[d].object.GetParent() == NULL)
		{
			PlaySound(SOUND_LABEL_SE_ABSORB);
			debris[d].use = false;
			flag_score += 1;

			// �y�A�����g�����o���b�g�̉������
			for (int b = 0; b < MAX_BULLET; b++)
			{			
				// �e�ۂ̎g�p�t���O���m�F
				if (!bullet[b].use)continue;
				if (debris[d].object.GetParent())
				{
					debris[d].object.SetParent(NULL);
					bullet[b].use = false;
					bullet[b].spd = 1.0f;
				}
			}
		}
		//�G�t�F�N�g�̃C���[�W�͋z�����܂�銴��(�}�C�N���̌o���l���߂�)
	}

	// �A���e�i�̂Ə���
	for (int a = 0; a < MAX_ANTENNA; a++)
	{
		//�A���e�i�̎g�p�t���O���`�F�b�N
		if (!antenna[a].use)continue;
		//�g�p�t���O���`�F�b�N��������W���Q�b�g
		antennaPos = antenna[a].object.GetPositionFloat();

		// �v���C���[�ƃA���e�i�̓����蔻��
		if (!CollisionBC(playerPos, antennaPos, player->size, antenna[a].size)) continue;
		// �v���C���[�ƃA���e�i�̓����蔽���i�^�j
		if (!antenna[a].object.GetParent() == NULL)
		{
			PlaySound(SOUND_LABEL_SE_ABSORB);
			antenna[a].use = false;
			flag_score += 5;
			
			// �y�A�����g�����o���b�g�̉������
			for (int b = 0; b < MAX_BULLET; b++)
			{
				// �e�ۂ̎g�p�t���O���m�F
				if (!bullet[b].use)continue;
				if (antenna[a].object.GetParent())
				{
					antenna[a].object.SetParent(NULL);
					bullet[b].use = false;
					bullet[b].spd = 1.0f;
				}
			}
		}
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
