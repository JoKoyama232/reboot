// �f�u������ [solarpanel.cpp]
// Author ����l�@��
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "solarpanel.h"
#include "sprite.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PANEL		"Data/model/chipped2.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���

#define TEXTURE_MAX			(1)								// �e�N�X�`���̐�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


static float        g_LastUpdate;
PANEL g_Panel[MAX_PANEL];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitPanel(void) {

	for (int i = 0; i < MAX_PANEL; i++)
	{
		LoadModel(MODEL_PANEL, &g_Panel[i].object.modelInfo);
		GetModelDiffuse(&g_Panel[i].object.modelInfo, g_Panel[i].object.modelDiffuse);
		g_Panel[i].object.load = true;
		g_Panel[i].object.SetPosition(XMFLOAT3{ rand() % 1000 + 0.0f, 0.0f, rand() % 1000 + 0.0f });
		g_Panel[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Panel[i].object.SetScale(XMFLOAT3{ 0.4f, 0.4f, 0.4f });
		g_Panel[i].time = 0.0f;
		g_Panel[i].speed = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Panel[i].use = true;
		g_Panel[i].flag_rotate = true;
		g_Panel[i].object.draw = true;
		g_Panel[i].size = PANEL_SIZE;
		g_LastUpdate = 0.0f;

	}
	return S_OK;

}

void UninitPanel(void) {
	// ���f���̉������
	for (int i = 0; i < MAX_PANEL; i++)
	{

		if (g_Panel[i].object.load)
		{
			UnloadModel(&g_Panel[i].object.modelInfo);
			g_Panel[i].object.load = false;
		}
	}
}

void UpdatePanel(void) {
	// �f�u���ϐ��擾
	for (int i = 0; i < MAX_PANEL; i++)
	{
		// �f�u���̎g�p�t���O���m�F
		if (!g_Panel[i].use)
		{
			g_Panel[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_Panel[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Panel[i].object.GetRotationFloat();

		if (!g_Panel[i].flag_rotate)continue;
		{
			// ���邮���]
			rotation.x += (i + 1) * 0.005f;
			rotation.y += ((rand() % 5) + 1) * 0.001f;
			rotation.z += ((MAX_PANEL + 1) - i) * 0.005f;
		}

		// �ړ���]�𔽉f
		g_Panel[i].object.SetPosition(position);
		g_Panel[i].object.SetRotation(rotation);
	}

}

void DrawPanel(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_PANEL; i++)
	{
		if (!g_Panel[i].object.draw) continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		XMFLOAT3 scale = g_Panel[i].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		XMFLOAT3 rotation = g_Panel[i].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		XMFLOAT3 position = g_Panel[i].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Panel[i].object.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_Panel[i].object.modelInfo);

		SetFuchi(0);

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);

	}
}

PANEL* GetPanel(void) {

	return &g_Panel[0];
}

