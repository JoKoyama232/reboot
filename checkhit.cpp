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
#include "playerUI.h"
#include "game.h"
#include "checkhit.h"



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

static int flag_score = 0;
static int flag_bonus = 0;

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	// �e�I�u�W�F�N�g�̈ʒu���`
	XMFLOAT3 podPos, basePos, hatchPos, panelPos, playerPos,
		debrisPos, bulletPos, rocketPos, antennaPos, satellitePos;

	//�e�I�u�W�F�N�g�̃X�P�[�����`
	XMFLOAT3 playerScale, baseScale, bulletScale, debriScale,
		antennaScale, podScale, panelScale, hatchScale, satelliteScale, rocketScale;

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
	playerScale = GetPlayer()->object.GetScaleFloat();
	basePos = GetBase()->object.GetPositionFloat();
	baseScale = GetBase()->object.GetScaleFloat();
	// �v���C���[���g���Ă��Ȃ��̂Ȃ瓖���蔻����K�v�Ȃ����߂܂��v���C���[�̎g�p�t���O�m�F
	if (!player->use) return;

	// �e�ۖ��̓����蔻��----------------------------------------------------
	for (int cntBullet = 0; cntBullet < MAX_BULLET; cntBullet++)
	{
		
		// �e�ۂ̎g�p�t���O���m�F
		if (!bullet[cntBullet].use) continue;
		bulletPos = bullet[cntBullet].object.GetPositionFloat();
		bulletScale = bullet[cntBullet].object.GetScaleFloat();

		// �t���O�m�F
		bullet[cntBullet].flag_Halpha = false;

		// �o���b�g�΃f�u�������蔻��
		for (int cntDebris = 0; cntDebris < MAX_DEBRIS; cntDebris++)
		{
			// �f�u���̎g�p�t���O�m�F
			if (!debris[cntDebris].use) continue;

			//�g�p�t���O���`�F�b�N��������W�ƃX�P�[�����Q�b�g
			debrisPos = debris[cntDebris].object.GetPositionFloat();
			debriScale = debris[cntDebris].object.GetScaleFloat();
			// �e�ۂƃf�u���̓����蔻��
			if (!CheckDebris(bulletPos, bulletScale, debrisPos, debriScale)) continue;
			// �e�ۂƃf�u���̓����蔽���i�^
			bullet[cntBullet].flag_Halpha = true;
			bullet[cntBullet].spd = 0.0f;
			debris[cntDebris].flag_rotate = false;
			debris[cntDebris].object.SetParent(&bullet[cntBullet].object);
			if (bullet[cntBullet].Halpha <= 0.0f)
			{
				bullet[cntBullet].flag_Halpha = false;
			}
			
		}



		//�o���b�g�ƃA���e�i�̓����蔻��
		for (int cntAntn = 0; cntAntn < MAX_ANTENNA; cntAntn++)
		{
			//�A���e�i�̎g�p�t���O���`�F�b�N
			if (!antenna[cntAntn].use)continue;

			//�g�p�t���O���`�F�b�N��������W���Q�b�g
			antennaPos = antenna[cntAntn].object.GetPositionFloat();
			antennaScale = antenna[cntAntn].object.GetScaleFloat();

			// �e�ۂƃA���e�i�̓����蔻��
			// X��������������CheckLX���g�p
			if (!CheckLX(bulletPos, bulletScale, antennaPos, antennaScale))continue;
			bullet[cntBullet].spd = 0.0f;
			bullet[cntBullet].flag_Halpha = true;
			antenna[cntAntn].flag_rotate = false;
			antenna[cntAntn].object.SetParent(&bullet[cntBullet].object);
			if (bullet[cntBullet].Halpha <= 0.0f)
			{
				bullet[cntBullet].flag_Halpha = false;
			}

		}

		// �o���b�g�ƃ|�b�h�̓����蔻��
		for (int cntPod = 0; cntPod < MAX_POD; cntPod++)
		{
			// �|�b�h�̎d�l�t���O���`�F�b�N
			if (!pod[cntPod].use)continue;

			//�d�l�t���O���`�F�b�N��������W���Q�b�g
			podPos = pod[cntPod].object.GetPositionFloat();
			podScale = pod[cntPod].object.GetScaleFloat();

			// �o���b�g�ƃ|�b�h�̓����蔻��
			if (!CheckDebris(bulletPos, bulletScale, podPos, podScale))continue;
			bullet[cntBullet].spd = 0.0f;
			bullet[cntBullet].flag_Halpha = true;
			pod[cntPod].flag_rotate = false;
			pod[cntPod].object.SetParent(&bullet[cntBullet].object);
			if (bullet[cntBullet].Halpha <= 0.0f)
			{
				bullet[cntBullet].flag_Halpha = false;
			}

		}

		// �o���b�g�ƃ\�|���[�p�l���̓����蔻��
		for (int cntPanel = 0; cntPanel < MAX_PANEL; cntPanel++)
		{
			// �\�[���[�p�l���̎d�l�t���O���`�F�b�N
			if (!panel[cntPanel].use)continue;

			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			panelPos = panel[cntPanel].object.GetPositionFloat();
			panelScale = panel[cntPanel].object.GetScaleFloat();

			// �o���b�g�ƃp�l���̓����蔻��
			if (!CheckDebris(bulletPos, bulletScale, panelPos, panelScale))continue;
			bullet[cntBullet].spd = 0.0f;		
			bullet[cntBullet].flag_Halpha = true;
			panel[cntPanel].flag_rotate = false;
			panel[cntPanel].object.SetParent(&bullet[cntBullet].object);
			if (bullet[cntBullet].Halpha <= 0.0f)
			{
				bullet[cntBullet].flag_Halpha = false;
			}

		}

		// �o���b�g�ƃn�b�`�̓����蔻��
		for (int cntHatch = 0; cntHatch < MAX_HATCH; cntHatch++)
		{
			// �n�b�`�Ɏd�l�t���O���`�F�b�N
			if (!hatch[cntHatch].use)continue;

			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			hatchPos = hatch[cntHatch].object.GetPositionFloat();
			hatchScale = hatch[cntHatch].object.GetScaleFloat();

			// �o���b�g�ƃn�b�`�̓����蔻��
			if (!CheckDebris(bulletPos, bulletScale, hatchPos, hatchScale))continue;
			bullet[cntBullet].spd = 0.0f;
			bullet[cntBullet].flag_Halpha = true;
			hatch[cntHatch].flag_rotate = false;
			hatch[cntHatch].object.SetParent(&bullet[cntBullet].object);
			if (bullet[cntBullet].Halpha <= 0.0f)
			{
				bullet[cntBullet].flag_Halpha = false;
			}


		}

		// �o���b�g�Ɛl�H�q���̓����蔻��
		for (int cntSL = 0; cntSL < MAX_SATELLITE; cntSL++)
		{
			// �l�H�q���̎g�p�t���O���`�F�b�N
			if (!satellite[cntSL].use)continue;

			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			satellitePos = satellite[cntSL].object.GetPositionFloat();
			satelliteScale = satellite[cntSL].object.GetScaleFloat();

			// �o���b�g�Ɛl�H�q���̓����蔻��
			// Z��������������CheckLZ���g�p
			if (!CheckLZ(bulletPos, bulletScale, satellitePos, satelliteScale))continue;
			bullet[cntBullet].spd = 0.0f;
			bullet[cntBullet].flag_Halpha = true;
			satellite[cntSL].flag_rotate = false;
			satellite[cntSL].object.SetParent(&bullet[cntBullet].object);
			if (bullet[cntBullet].Halpha <= 0.0f)
			{
				bullet[cntBullet].flag_Halpha = false;
			}

		}

		// �o���b�g�ƃ��P�b�g�̓����蔻��
		for (int cntRocket = 0; cntRocket < MAX_ROCKET; cntRocket++)
		{
			// ���P�b�g�̎d�l�t���O���`�F�b�N
			if (!rocket[cntRocket].use)continue;

			// �d�l�t���O���`�F�b�N��������W���Q�b�g
			rocketPos = rocket[cntRocket].object.GetPositionFloat();
			rocketScale = rocket[cntRocket].object.GetScaleFloat();

			// �o���b�g�ƃ��P�b�g�̓����蔻��
			if (!CheckLY(bulletPos, bulletScale, rocketPos, rocketScale))continue;
			bullet[cntBullet].spd = 0.0f;
			bullet[cntBullet].flag_Halpha = true;
			rocket[cntRocket].flag_rotate = false;
			rocket[cntRocket].object.SetParent(&bullet[cntBullet].object);
			if (bullet[cntBullet].Halpha <= 0.0f)
			{
				bullet[cntBullet].flag_Halpha = false;
			}

		}

		if (bullet[cntBullet].use == false)
		{
			bullet[cntBullet].flag_Halpha = false;
			bullet[cntBullet].Halpha = 1.0f;
		}
	}

	//----------------------------------------------------------------------



	// �v���C���[�̓����蔻��-------------------------------------------------
	bool unload = false;		// �ێ����Ă���f�u�������
	// �v���C���[�Ɗ�n�̓����蔻��
	if (CheckBase(playerPos, playerScale, basePos, baseScale))
	{
		// �v���C���[�Ɗ�n�̓����蔽���i�^
		if (!flag_score == 0)
		{
			player->C2alpha = 1.0f;
			PlaySound(SOUND_LABEL_SE_SCORE);
			AddScore(flag_score * 100);
			AddBonus(flag_bonus * 50);
			flag_score = 0;
		}
		
		player->Ralpha = 1.0f;
		// E�L�[��������e��[������
		if (GetKeyboardTrigger(DIK_E))
		{
			for (int i = 0; i < MAX_BULLET; i++)
			{
				if (!bullet[i].spd == 1.0f)continue;
				{
					bullet[i].use = false;
				}
			}
		}
	}
	else
	{
		player->Ralpha = 0.0f;
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
			debriScale = debris[cntDebris].object.GetScaleFloat();

			// �v���C���[�ƃf�u���̓����蔻��
			if (!CheckPlayer(playerPos, playerScale, debrisPos, debriScale)) continue;

			// �v���C���[�ƃf�u���̓����蔽���i�^�j
			if (!debris[cntDebris].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);
				if (debris[cntDebris].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 1;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					debris[cntDebris].object.SetParent(NULL);
					debris[cntDebris].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].flag_Halpha = false;
					bullet[cntBullet].Halpha = 1.0f;
					bullet[cntBullet].spd = 5.0f;
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
			antennaScale = antenna[cntAntn].object.GetScaleFloat();

			// �v���C���[�ƃA���e�i�̓����蔻��
			if (!CheckPlayer(playerPos, playerScale, antennaPos, antennaScale)) continue;

			// �v���C���[�ƃA���e�i�̓����蔽���i�^�j
			if (!antenna[cntAntn].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (antenna[cntAntn].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 5;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					antenna[cntAntn].object.SetParent(NULL);
					antenna[cntAntn].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].flag_Halpha = false;
					bullet[cntBullet].Halpha = 1.0f;
					bullet[cntBullet].spd = 5.0f;
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
			podScale = pod[cntPod].object.GetScaleFloat();

			// �v���C��\�ƃ|�b�h�̓����蔻��
			if (!CheckPlayer(playerPos, playerScale, podPos, podScale)) continue;

			// �v���C��\�ƃ|�b�h�̓����蔻��(�^)
			if (!pod[cntPod].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (pod[cntPod].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					pod[cntPod].object.SetParent(NULL);
					pod[cntPod].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].flag_Halpha = false;
					bullet[cntBullet].Halpha = 1.0f;
					bullet[cntBullet].spd = 5.0f;
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
			panelScale = panel[cntPanel].object.GetScaleFloat();

			//�v���C���[�ƃp�l���̓����蔻��
			if (!CheckPlayer(playerPos, playerScale, panelPos, panelScale)) continue;

			//�v���C���[�ƃp�l���̓����蔻��(�^)
			if (!panel[cntPanel].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (panel[cntPanel].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					panel[cntPanel].object.SetParent(NULL);
					panel[cntPanel].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].flag_Halpha = false;
					bullet[cntBullet].Halpha = 1.0f;
					bullet[cntBullet].spd = 5.0f;
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
			hatchScale = hatch[cntHatch].object.GetScaleFloat();

			// �v���C���[�ƃn�b�`�̓����蔻��
			if (!CheckPlayer(playerPos, hatchPos, playerScale, hatchScale)) continue;

			// �v���C���[�ƃn�b�`�̓����蔻��(�^)
			if (!hatch[cntHatch].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (hatch[cntHatch].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					hatch[cntHatch].object.SetParent(NULL);
					hatch[cntHatch].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].flag_Halpha = false;
					bullet[cntBullet].Halpha = 1.0f;
					bullet[cntBullet].spd = 5.0f;
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
			satelliteScale = satellite[cntSL].object.GetScaleFloat();

			// �v���C���[�Ɛl�H�q���̓����蔻��
			if (!CheckPlayer(playerPos, playerScale, satellitePos, satelliteScale)) continue;

			// �v���C���[�Ɛl�H�q���̓����蔻��(�^)
			if (!satellite[cntSL].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (satellite[cntSL].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					satellite[cntSL].object.SetParent(NULL);
					satellite[cntSL].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].flag_Halpha = false;
					bullet[cntBullet].Halpha = 1.0f;
					bullet[cntBullet].spd = 5.0f;
				}
			}
		}
		//----------------------------------------------------------------------


		for (int cntRocket = 0; cntRocket < MAX_ROCKET; cntRocket++)
		{
			// ���P�b�g�̎g�p�t���O���`�F�b�N
			if (!rocket[cntRocket].use)continue;

			// �g�p�t���O���`�F�b�N��������W���Q�b�g
			rocketPos = rocket[cntRocket].object.GetPositionFloat();
			rocketScale = rocket[cntRocket].object.GetScaleFloat();

			// �v���C���[�ƃ��P�b�g�̓����蔻��
			if (!CheckPlayer(playerPos, playerScale, rocketPos, rocketScale)) continue;

			// �v���C���[�ƃ��P�b�g�̓����蔻��(�^)
			if (!rocket[cntRocket].object.GetParent() == NULL)
			{
				PlaySound(SOUND_LABEL_SE_ABSORB);

				if (rocket[cntRocket].object.GetParent() == &bullet[cntBullet].object)
				{
					flag_score += 3;
					flag_bonus += 1;
					player->Calpha = 1.0f;
					rocket[cntRocket].object.SetParent(NULL);
					rocket[cntRocket].use = false;
					bullet[cntBullet].use = false;
					bullet[cntBullet].flag_Halpha = false;
					bullet[cntBullet].Halpha = 1.0f;
					bullet[cntBullet].spd = 5.0f;
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


//=============================================================================
// ���_�ƃv���C���[�̓����蔻��̌v�Z
//=============================================================================
BOOL CheckBase(XMFLOAT3 pos1, XMFLOAT3 scale1, XMFLOAT3 pos2, XMFLOAT3 scale2)
{
	BOOL ans = FALSE;		//�͂���̃Z�b�g
	
	// base�ɍ����悤�ɃX�P�[���𒲐�
	scale1.x *= 20.0f;
	scale1.y *= 4.0f;
	scale1.z *= 2.0f;
	scale2.x *= 20.0f;
	scale2.y *= 4.0f;
	scale2.z *= 6.0f;

	if ((pos1.x + scale1.x > pos2.x - scale2.x) &&
		(pos1.x - scale1.x < pos2.x + scale2.x) &&
		(pos1.y + scale1.y > pos2.y - scale2.y) &&
		(pos1.y - scale1.y < pos2.y + scale2.y) &&
		(pos1.z + scale1.z > pos2.z - scale2.z) &&
		(pos1.z - scale1.z < pos2.z + scale2.z))
	{
		ans = TRUE;	// �������Ă���
	}

	return ans;
}

//=============================================================================
// �g�����`�ƃf�u���̓����蔻��̌v�Z
// �l�p����͂�����������
//=============================================================================
BOOL CheckPlayer(XMFLOAT3 ppos, XMFLOAT3 pscale, XMFLOAT3 dpos, XMFLOAT3 dscale)
{
	BOOL ans = FALSE;	// �͂���̃Z�b�g

	//�f�̐��l���Ə���������̂Ōv�Z�������X�P�[���𑝂₷
	pscale.x *= 10.0f;
	pscale.y *= 10.0f;
	pscale.z *= 10.0f;
	dscale.x *= 10.0f;
	dscale.y *= 10.0f;
	dscale.z *= 10.0f;

	if ((ppos.x + pscale.x > dpos.x - dscale.x) &&
		(ppos.x - pscale.x < dpos.x + dscale.x) &&
		(ppos.y + pscale.y > dpos.y - dscale.y) &&
		(ppos.y - pscale.y < dpos.y + dscale.y) &&
		(ppos.z + pscale.z > dpos.z - dscale.z) &&
		(ppos.z - pscale.z < dpos.z + dscale.z))
	{
		ans = TRUE;
	}

	return ans;
}


//=============================================================================
// �g�����`�ƃf�u���̓����蔻��̌v�Z
// �l�p����͂�����������
//=============================================================================
BOOL CheckDebris(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 dPos, XMFLOAT3 dScale)
{
	BOOL ans = FALSE;	// �͂���̃Z�b�g

	//�f�̐��l���Ə���������̂Ōv�Z�������X�P�[���𑝂₷
	bScale.x *= 5.0f;
	bScale.y *= 5.0f;
	bScale.z *= 5.0f;
	dScale.x *= 5.0f;
	dScale.y *= 5.0f;
	dScale.z *= 5.0f;

	if ((bPos.x + bScale.x > dPos.x - dScale.x) &&
		(bPos.x - bScale.x < dPos.x + dScale.x) &&
		(bPos.y + bScale.y > dPos.y - dScale.y) &&
		(bPos.y - bScale.y < dPos.y + dScale.y) &&
		(bPos.z + bScale.z > dPos.z - dScale.z) &&
		(bPos.z - bScale.z < dPos.z + dScale.z))
	{
		ans = TRUE;
	}

	return ans;
}

//=============================================================================
// LongX(X������������)�̓����蔻��
//=============================================================================
BOOL CheckLX(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 xPos, XMFLOAT3 xScale)
{
	BOOL ans = FALSE;	// �͂���̃Z�b�g

	// x���ɒ����̂ł���𒲐�
	bScale.x *= 10.0f;
	bScale.y *= 3.0f;
	bScale.z *= 3.0f;
	xScale.x *= 10.0f;
	xScale.y *= 3.0f;
	xScale.z *= 3.0f;

	if ((bPos.x + bScale.x > xPos.x - xScale.x) &&
		(bPos.x - bScale.x < xPos.x + xScale.x) &&
		(bPos.y + bScale.y > xPos.y - xScale.y) &&
		(bPos.y - bScale.y < xPos.y + xScale.y) &&
		(bPos.z + bScale.z > xPos.z - xScale.z) &&
		(bPos.z - bScale.z < xPos.z + xScale.z))
	{
		ans = TRUE;
	}

	return ans;
}

//=============================================================================
// LongY(Y������������)�̓����蔻��
//=============================================================================
BOOL CheckLY(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 yPos, XMFLOAT3 yScale)
{
	BOOL ans = FALSE;	// �͂���̃Z�b�g

	// x���ɒ����̂ł���𒲐�
	bScale.x *= 3.0f;
	bScale.y *= 10.0f;
	bScale.z *= 3.0f;
	yScale.x *= 3.0f;
	yScale.y *= 10.0f;
	yScale.z *= 3.0f;

	if ((bPos.x + bScale.x > yPos.x - yScale.x) &&
		(bPos.x - bScale.x < yPos.x + yScale.x) &&
		(bPos.y + bScale.y > yPos.y - yScale.y) &&
		(bPos.y - bScale.y < yPos.y + yScale.y) &&
		(bPos.z + bScale.z > yPos.z - yScale.z) &&
		(bPos.z - bScale.z < yPos.z + yScale.z))
	{
		ans = TRUE;
	}

	return ans;
}


//=============================================================================
// LongZ(Z������������)�̓����蔻��
//=============================================================================
BOOL CheckLZ(XMFLOAT3 bPos, XMFLOAT3 bScale, XMFLOAT3 zPos, XMFLOAT3 zScale)
{
	BOOL ans = FALSE;	// �͂���̃Z�b�g

	// z���ɒ����̂ł���𒲐�
	bScale.x *= 3.0f;
	bScale.y *= 3.0f;
	bScale.z *= 10.0f;
	zScale.x *= 3.0f;
	zScale.y *= 3.0f;
	zScale.z *= 10.0f;

	if ((bPos.x + bScale.x > zPos.x - zScale.x) &&
		(bPos.x - bScale.x < zPos.x + zScale.x) &&
		(bPos.y + bScale.y > zPos.y - zScale.y) &&
		(bPos.y - bScale.y < zPos.y + zScale.y) &&
		(bPos.z + bScale.z > zPos.z - zScale.z) &&
		(bPos.z - bScale.z < zPos.z + zScale.z))
	{
		ans = TRUE;
	}

	return ans;
}