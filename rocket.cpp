// �f�u������ [rocket.cpp]
// Author ����l�@��
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "rocket.h"
#include "sprite.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ROCKET		"Data/model/rocket.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���

#define TEXTURE_MAX			(1)								// �e�N�X�`���̐�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


static float        g_LastUpdate;
ROCKET g_Rocket[MAX_ROCKET];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitRocket(void) {

	for (int i = 0; i < MAX_ROCKET; i++)
	{
		LoadModel(MODEL_ROCKET, &g_Rocket[i].object.modelInfo);
		GetModelDiffuse(&g_Rocket[i].object.modelInfo, g_Rocket[i].object.modelDiffuse);
		g_Rocket[i].object.load = true;
		g_Rocket[i].object.SetPosition(XMFLOAT3{ rand() % 1000 + 0.0f, 0.0f, rand() % 1000 + 0.0f });
		g_Rocket[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Rocket[i].object.SetScale(XMFLOAT3{ 0.3f, 0.3f, 0.3f });
		g_Rocket[i].time = 0.0f;
		g_Rocket[i].speed = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Rocket[i].use = true;
		g_Rocket[i].object.draw = true;
		g_Rocket[i].size = ROCKET_SIZE;
		g_LastUpdate = 0.0f;

	}
	return S_OK;

}

void UninitRocket(void) {
	// ���f���̉������
	for (int i = 0; i < MAX_ROCKET; i++)
	{

		if (g_Rocket[i].object.load)
		{
			UnloadModel(&g_Rocket[i].object.modelInfo);
			g_Rocket[i].object.load = false;
		}
	}
}

void UpdateRocket(void) {
	// �f�u���ϐ��擾
	for (int i = 0; i < MAX_ROCKET; i++)
	{
		// �f�u���̎g�p�t���O���m�F
		if (!g_Rocket[i].use)
		{
			g_Rocket[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_Rocket[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Rocket[i].object.GetRotationFloat();



		// ���邮���]
		rotation.x += (i + 1) * 0.005f;
		rotation.y += ((rand() % 5) + 1) * 0.001f;
		rotation.z += ((MAX_ROCKET + 1) - i) * 0.005f;

		// �ړ���]�𔽉f
		g_Rocket[i].object.SetPosition(position);
		g_Rocket[i].object.SetRotation(rotation);
	}

}

void DrawRocket(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ROCKET; i++)
	{
		if (!g_Rocket[i].object.draw) continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		XMFLOAT3 scale = g_Rocket[i].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		XMFLOAT3 rotation = g_Rocket[i].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		XMFLOAT3 position = g_Rocket[i].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Rocket[i].object.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_Rocket[i].object.modelInfo);

		SetFuchi(0);

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);

	}
}

ROCKET* GetRocket(void) {

	return &g_Rocket[0];
}

