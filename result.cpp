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
#include "result.h"
#include "result_tex.h"
#include "Skybox.h"

//#include "title_field.h"
//#include "model.h"
//#include "title_model.h"

//=============================================================================
// ����������
//=============================================================================

HRESULT InitResult(void)
{
	InitResultTex();

	InitSkybox();

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	UninitResultTex();

	UninitSkybox();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	UpdateResultTex();

	UpdateSkybox();

#ifdef _DEBUG
	if (GetKeyState(VK_RBUTTON) & 0x80)
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
#endif // _DEBUG

}



//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	DrawSkybox();





	// 2D�̕���`�悷�鏈��
	{
		// Z��r�Ȃ�
		SetDepthEnable(false);

		// ���C�e�B���O�𖳌�
		SetLightEnable(false);

		//�������牺��
		DrawResultTex();

		// ���C�e�B���O��L����
		SetLightEnable(true);

		// Z��r����
		SetDepthEnable(true);
	}
}