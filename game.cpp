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

	DrawPod();

	DrawPanel();

	DrawHatch();

	DrawSatellite();

	//DrawRocket();

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
	for (int cntBullet = 0; cntBullet < MAX_BULLET; cntBullet++)
	{
		// �e�ۂ̎g�p�t���O���m�F
		if (!bullet[cntBullet].use) continue;
		bulletPos = bullet[cntBullet].object.GetPositionFloat();

		// �o���b�g�΃f�u�������蔻��
		for (int cntDebris = 0; cntDebris < MAX_DEBRIS; cntDebris++)
		{
			// �f�u���̎g�p�t���O�m�F
			if (!debris[cntDebris].use) continue;

			//�g�p�t���O���`�F�b�N��������W���Q�b�g
			debrisPos = debris[cntDebris].object.GetPositionFloat();

			// �e�ۂƃf�u���̓����蔻��
			if (!CollisionBC(bulletPos, debrisPos, bullet[cntBullet].size, debris[cntDebris].size)) continue;

			// �e�ۂƃf�u���̓����蔽���i�^�j
			bullet[cntBullet].spd = 0.0f;
			debris[cntDebris].flag_rotate = false;
			debris[cntDebris].object.SetParent(&bullet[cntBullet].object);
		}


		//�o���b�g�ƃA���e�i�̓����蔻��
		for (int cntAntn = 0; cntAntn < MAX_ANTENNA; cntAntn++)
		{
			//�A���e�i�̎g�p�t���O���`�F�b�N
			if (!antenna[cntAntn].use)continue;

			//�g�p�t���O���`�F�b�N��������W���Q�b�g
			antennaPos = antenna[cntAntn].object.GetPositionFloat();

			// �e�ۂƃA���e�i�̓����蔻��
			if (!CollisionBC(bulletPos, antennaPos, bullet[cntBullet].size, antenna[cntAntn].size))continue;

			bullet[cntBullet].spd = 0.0f;
			antenna[cntAntn].flag_rotate = false;
			antenna[cntAntn].object.SetParent(&bullet[cntBullet].object);
		}

		// �o���b�g�ƃ|�b�h�̓����蔻��
		for (int cntPod = 0; cntPod < MAX_POD; cntPod++)
		{
			// �|�b�h�̎d�l�t���O���`�F�b�N
			if (!pod[cntPod].use)continue;

			//�d�l�t���O���`�F�b�N��������W���Q�b�g
			podPos = pod[cntPod].object.GetPositionFloat();

			// �o���b�g�ƃ|�b�h�̓����蔻��
			if (!CollisionBC(bulletPos, podPos, bullet[cntBullet].size, pod[cntPod].size))continue;

			bullet[cntBullet].spd = 0.0f;
			pod[cntPod].flag_rotate = false;
			pod[cntPod].object.SetParent(&bullet[cntBullet].object);
		}

		// �o���b�g�ƃ\�|���[�p�l���̓����蔻��
		for (int cntPanel = 0; cntPanel < MAX_PANEL; cntPanel++)
		{
			// �\�[���[�p�l���̎d�l�t���O���`�F�b�N
			if (!panel[cntPanel].use)continue;
			
			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			panelPos = panel[cntPanel].object.GetPositionFloat();

			// �o���b�g�ƃp�l���̓����蔻��
			if (!CollisionBC(bulletPos, panelPos, bullet[cntBullet].size, panel[cntPanel].size))continue;
		
			bullet[cntBullet].spd = 0.0f;
			panel[cntPanel].flag_rotate = false;
			panel[cntPanel].object.SetParent(&bullet[cntBullet].object);		
		}

		// �o���b�g�ƃn�b�`�̓����蔻��
		for (int cntHatch = 0; cntHatch < MAX_HATCH; cntHatch++)
		{
			// �n�b�`�Ɏd�l�t���O���`�F�b�N
			if (!hatch[cntHatch].use)continue;
			
			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			hatchPos = hatch[cntHatch].object.GetPositionFloat();
			
			// �o���b�g�ƃn�b�`�̓����蔻��
			if (!CollisionBC(bulletPos, hatchPos, bullet[cntBullet].size, hatch[cntHatch].size))continue;
			
			bullet[cntBullet].spd = 0.0f;
			hatch[cntHatch].flag_rotate = false;
			hatch[cntHatch].object.SetParent(&bullet[cntBullet].object);
			
		}

		// �o���b�g�Ɛl�H�q���̓����蔻��
		for (int cntSL = 0; cntSL < MAX_SATELLITE; cntSL++)
		{
			// �l�H�q���̎g�p�t���O���`�F�b�N
			if (!satellite[cntSL].use)continue;

			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			satellitePos = satellite[cntSL].object.GetPositionFloat();

			// �o���b�g�Ɛl�H�q���̓����蔻��
			if (!CollisionBC(bulletPos, satellitePos, bullet[cntBullet].size, satellite[cntSL].size))continue;

			bullet[cntBullet].spd = 0.0f;
			satellite[cntSL].flag_rotate = false;
			satellite[cntSL].object.SetParent(&bullet[cntBullet].object);
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
	// �y�A�����g�����o���b�g�̉������
	for (int cntBullet = 0; cntBullet < MAX_BULLET; cntBullet++)
	{
		// �e�ۂ̎g�p�t���O���m�F
		if (!bullet[cntBullet].use)continue;

		// �f�u���Ƃ̏���
		for (int cntDebris = 0; cntDebris < MAX_DEBRIS; cntDebris++)
		{
			// �f�u���̎g�p�t���O�m�F
			if (!debris[cntDebris].use) continue;

			//�g�p�t���O���`�F�b�N��������W���Q�b�g
			debrisPos = debris[cntDebris].object.GetPositionFloat();

			// �v���C���[�ƃf�u���̓����蔻��
			if (!CollisionBC(playerPos, debrisPos, player->size, debris[cntDebris].size)) continue;

			// �v���C���[�ƃf�u���̓����蔽���i�^�j
			if (!debris[cntDebris].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 1;

				if (debris[cntDebris].object.GetParent() == &bullet[cntBullet].object)
				{
					debris[cntDebris].object.SetParent(NULL);
					debris[cntDebris].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 1.0f;
				}
				
			}
			//�G�t�F�N�g�̃C���[�W�͋z�����܂�銴��(�}�C�N���̌o���l���߂�)
		}
		//----------------------------------------------------------------------

		
		// �A���e�i�̂Ə���
		for (int cntAntn = 0; cntAntn < MAX_ANTENNA; cntAntn++)
		{
			//�A���e�i�̎g�p�t���O���`�F�b�N
			if (!antenna[cntAntn].use)continue;
			//�g�p�t���O���`�F�b�N��������W���Q�b�g
			antennaPos = antenna[cntAntn].object.GetPositionFloat();

			// �v���C���[�ƃA���e�i�̓����蔻��
			if (!CollisionBC(playerPos, antennaPos, player->size, antenna[cntAntn].size)) continue;
			
			// �v���C���[�ƃA���e�i�̓����蔽���i�^�j
			if (!antenna[cntAntn].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 5;

				if (antenna[cntAntn].object.GetParent() == &bullet[cntBullet].object)
				{
					antenna[cntAntn].object.SetParent(NULL);
					antenna[cntAntn].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 1.0f;
				}
			}
		}
		//----------------------------------------------------------------------



		// �|�b�h�Ƃ̏���
		for (int cntPod = 0; cntPod < MAX_POD; cntPod++)
		{
			// �|�b�h�̎g�p�t���O���`�F�b�N
			if (!pod[cntPod].use)continue;
			//�d�l�t���O���m�F��������W���Q�b�g
			podPos = pod[cntPod].object.GetPositionFloat();

			// �v���C��\�ƃ|�b�h�̓����蔻��
			if (!CollisionBC(playerPos, podPos, player->size, pod[cntPod].size)) continue;
			
			// �v���C��\�ƃ|�b�h�̓����蔻��(�^)
			if (!pod[cntPod].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 3;

				if (pod[cntPod].object.GetParent() == &bullet[cntBullet].object)
				{
					pod[cntPod].object.SetParent(NULL);
					pod[cntPod].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 1.0f;
				}
			}
		}
		//----------------------------------------------------------------------


		// �\�[���[�p�l���Ƃ̏���
		for (int cntPanel = 0; cntPanel < MAX_PANEL; cntPanel++)
		{
			// �p�l���̎d�l�t���O���`�F�b�N
			if (!panel[cntPanel].use)continue;
			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			panelPos = panel[cntPanel].object.GetPositionFloat();

			//�v���C���[�ƃp�l���̓����蔻��
			if (!CollisionBC(playerPos, panelPos, player->size, panel[cntPanel].size)) continue;
			
			//�v���C���[�ƃp�l���̓����蔻��(�^)
			if (!panel[cntPanel].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 3;

				if (panel[cntPanel].object.GetParent() == &bullet[cntBullet].object)
				{
					panel[cntPanel].object.SetParent(NULL);
					panel[cntPanel].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 1.0f;
				}
			}
		}
		//----------------------------------------------------------------------


		// �n�b�`�Ƃ̏���
		for (int cntHatch = 0; cntHatch < MAX_HATCH; cntHatch++)
		{
			// �n�b�`�̎d�l�t���O���`�F�b�N
			if (!hatch[cntHatch].use)continue;

			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			hatchPos = hatch[cntHatch].object.GetPositionFloat();

			// �v���C���[�ƃn�b�`�̓����蔻��
			if (!CollisionBC(playerPos, hatchPos, player->size, hatch[cntHatch].size)) continue;
		
			// �v���C���[�ƃn�b�`�̓����蔻��(�^)
			if (!hatch[cntHatch].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 3;

				if (hatch[cntHatch].object.GetParent() == &bullet[cntBullet].object)
				{
					hatch[cntHatch].object.SetParent(NULL);
					hatch[cntHatch].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 1.0f;
				}
			}
		}
		//----------------------------------------------------------------------


		// �l�H�q���Ƃ̏���
		for (int cntSL = 0; cntSL < MAX_SATELLITE; cntSL++)
		{
			// �l�H�q���̎g�p�t���O���`�F�b�N
			if (!satellite[cntSL].use)continue;

			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			satellitePos = satellite[cntSL].object.GetPositionFloat();

			// �v���C���[�Ɛl�H�q���̓����蔻��
			if (!satellite[cntSL].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				flag_score += 3;

				if (satellite[cntSL].object.GetParent() == &bullet[cntBullet].object)
				{
					satellite[cntSL].object.SetParent(NULL);
					satellite[cntSL].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].spd = 1.0f;
				}
			}
		}
		//----------------------------------------------------------------------



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
