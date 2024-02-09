// �f�u������ [antenna.cpp]
// Author ����l�@��
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "title_antenna.h"
#include "sprite.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ANTENNA		"Data/model/antenna.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���

#define TEXTURE_MAX			(1)								// �e�N�X�`���̐�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


static float        g_LastUpdate;
T_ANTENNA g_TAntenna[MAX_ANTENNA];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitTitleAntenna(void) {

	for (int i = 0; i < MAX_ANTENNA; i++)
	{
		LoadModel(MODEL_ANTENNA, &g_TAntenna[i].object.modelInfo);
		GetModelDiffuse(&g_TAntenna[i].object.modelInfo, g_TAntenna[i].object.modelDiffuse);
		g_TAntenna[i].object.load = true;
		g_TAntenna[i].object.SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_TAntenna[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_TAntenna[i].object.SetScale(XMFLOAT3{ 1.0f, 1.0f, 1.0f });
		g_TAntenna[i].time = 0.0f;
		g_TAntenna[i].speed = 0.0f;			// �ړ��X�s�[�h�N���A
		g_TAntenna[i].use = true;
		g_TAntenna[i].flag_rotate = true;
		g_TAntenna[i].object.draw = true;
		g_TAntenna[i].size = ANTENNA_SIZE;
		g_LastUpdate = 0.0f;

		//�^�C�g�������ʒu�̎w��
		g_TAntenna[0].object.SetPosition(XMFLOAT3{ 100.0f,  20.0f, 100.0f });
		g_TAntenna[1].object.SetPosition(XMFLOAT3{ 200.0f,-100.0f, 200.0f });
		g_TAntenna[2].object.SetPosition(XMFLOAT3{ -75.0f,   0.0f,   0.0f });
		g_TAntenna[3].object.SetPosition(XMFLOAT3{-100.0f,   0.0f, 400.0f });
		g_TAntenna[4].object.SetPosition(XMFLOAT3{   0.0f,   0.0f, 100.0f });
		g_TAntenna[5].object.SetPosition(XMFLOAT3{ 100.0f,-100.0f, 100.0f });
	}
	return S_OK;

}

void UninitTitleAntenna(void) {
	// ���f���̉������
	for (int i = 0; i < MAX_ANTENNA; i++)
	{

		if (g_TAntenna[i].object.load)
		{
			UnloadModel(&g_TAntenna[i].object.modelInfo);
			g_TAntenna[i].object.load = false;
		}
	}
}

void UpdateTitleAntenna(void) {
	// �f�u���ϐ��擾
	for (int i = 0; i < MAX_ANTENNA; i++)
	{
		// �f�u���̎g�p�t���O���m�F
		if (!g_TAntenna[i].use)
		{
			g_TAntenna[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_TAntenna[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_TAntenna[i].object.GetRotationFloat();

		if (!g_TAntenna[i].flag_rotate)continue;
		{
			// ��]������
			rotation.x += (i + 1) * 0.001f;
			rotation.y += ((rand() % 5) + 1) * 0.001f;
			rotation.z += ((MAX_ANTENNA + 1) - i) * 0.001f;
		}

		// �ړ���]�𔽉f
		g_TAntenna[i].object.SetPosition(position);
		g_TAntenna[i].object.SetRotation(rotation);
	}

}

void DrawTitleAntenna(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ANTENNA; i++)
	{
		if (!g_TAntenna[i].object.draw) continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		XMFLOAT3 scale = g_TAntenna[i].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		XMFLOAT3 rotation = g_TAntenna[i].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		XMFLOAT3 position = g_TAntenna[i].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_TAntenna[i].object.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_TAntenna[i].object.modelInfo);

		SetFuchi(0);

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);

	}
}

T_ANTENNA* GetTitleAntenna(void) {

	return &g_TAntenna[0];
}

