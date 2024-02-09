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
#include "playerUI.h"
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
#include "basearrow.h"

#include "game.h"
#include "checkhit.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

static bool	g_bPause = true;	// �|�[�YON/OFF
static RECT	g_windowPos;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	InitPlayer();

	InitPlayerUI();

	InitDebris();

	InitAntenna();

	InitPanel();

	InitHatch();

	InitPod();

	InitSatellite();

	InitRocket();

	InitBase();

	InitBullet();

	InitClock();

	InitScore();

	InitSkybox();

	InitArrow();

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

	UninitBullet();

	UninitClock();

	UninitScore();

	UninitArrow();
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

	UpdateBullet();

	UpdateClock();

	UpdateSkybox();

	UpdateArrow();

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

	DrawSkybox();

	DrawPlayer();

	DrawDebris();

	DrawAntenna();

	DrawPod();

	DrawPanel();

	DrawHatch();

	DrawSatellite();

	DrawRocket();

	DrawBase();

	DrawBullet();

	DrawArrow();

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

		DrawAttach();

		DrawCapture();

		DrawCollect();

		DrawReload();

		DrawReticle();

		DrawHit();

		DrawMeterBase();

		// ���C�e�B���O��L����
		SetLightEnable(true);

		// Z��r����
		SetDepthEnable(true);
	}

}

