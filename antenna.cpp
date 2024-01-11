// �f�u������ [antenna.cpp]
// Author ����l�@��
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "antenna.h"
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
ANTENNA g_Antenna[MAX_ANTENNA];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitAntenna(void) {

	for (int i = 0; i < MAX_ANTENNA; i++)
	{
		LoadModel(MODEL_ANTENNA, &g_Antenna[i].object.modelInfo);
		GetModelDiffuse(&g_Antenna[i].object.modelInfo, g_Antenna[i].object.modelDiffuse);
		g_Antenna[i].object.load = true;
		g_Antenna[i].object.SetPosition(XMFLOAT3{ rand() % 1000 + 0.0f, rand() % 1000 - 500.0f, rand() % 1000 + 0.0f });
		g_Antenna[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Antenna[i].object.SetScale(XMFLOAT3{ 3.0f, 3.0f, 3.0f });
		g_Antenna[i].time = 0.0f;
		g_Antenna[i].speed = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Antenna[i].use = true;
		g_Antenna[i].flag_rotate = true;
		g_Antenna[i].object.draw = true;
		g_Antenna[i].size = ANTENNA_SIZE;
		g_LastUpdate = 0.0f;

	}
	return S_OK;

}

void UninitAntenna(void) {
	// ���f���̉������
	for (int i = 0; i < MAX_ANTENNA; i++)
	{

		if (g_Antenna[i].object.load)
		{
			UnloadModel(&g_Antenna[i].object.modelInfo);
			g_Antenna[i].object.load = false;
		}
	}
}

void UpdateAntenna(void) {
	// �f�u���ϐ��擾
	for (int i = 0; i < MAX_ANTENNA; i++)
	{
		// �f�u���̎g�p�t���O���m�F
		if (!g_Antenna[i].use)
		{
			g_Antenna[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_Antenna[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Antenna[i].object.GetRotationFloat();

		if (!g_Antenna[i].flag_rotate)continue;
		{
			// ���邮���]
			rotation.x += (i + 1) * 0.001f;
			rotation.y += ((rand() % 5) + 1) * 0.001f;
			rotation.z += ((MAX_ANTENNA + 1) - i) * 0.001f;
		}

		// �ړ���]�𔽉f
		g_Antenna[i].object.SetPosition(position);
		g_Antenna[i].object.SetRotation(rotation);
	}

}

void DrawAntenna(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ANTENNA; i++)
	{
		if (!g_Antenna[i].object.draw) continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		XMFLOAT3 scale = g_Antenna[i].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		XMFLOAT3 rotation = g_Antenna[i].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		XMFLOAT3 position = g_Antenna[i].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Antenna[i].object.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_Antenna[i].object.modelInfo);

		SetFuchi(0);

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);

	}
}

ANTENNA* GetAntenna(void) {

	return &g_Antenna[0];
}

