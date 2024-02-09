//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "sprite.h"
#include "title.h"
#include "title_tex.h"
#include "title_debris.h"
#include "title_antenna.h"
#include "Skybox.h"

//=============================================================================
// ����������
//=============================================================================

HRESULT InitTitle(void)
{
	InitTitleTex();

	InitTitleDebris();

	InitTitleAntenna();

	InitSkybox();


	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	UninitTitleTex();

	UninitTitleDebris();

	UninitTitleAntenna();

	UninitSkybox();
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	UpdateTitleTex();

	UpdateTitleDebris();

	UpdateTitleAntenna();

	UpdateSkybox();
}



//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	DrawTitleDebris();

	DrawTitleAntenna();

	DrawSkybox();

	// 2D�̕���`�悷�鏈��
	{
		// Z��r�Ȃ�
		SetDepthEnable(false);

		// ���C�e�B���O�𖳌�
		SetLightEnable(false);

		//�������牺��
		DrawTitleTex();

		// ���C�e�B���O��L����
		SetLightEnable(true);

		// Z��r����
		SetDepthEnable(true);
	}
}