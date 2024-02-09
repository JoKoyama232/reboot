//=============================================================================
// ������_���� [base.h]
// Author : ��c�P
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "base.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_DEBRIS		"Data/model/home.obj"			// �ǂݍ��ރ��f����


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static float        g_LastUpdate;
BASE g_Base[MAX_BASE];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���

HRESULT InitBase(void)
{
	for (int i = 0; i < MAX_BASE; i++)
	{
		LoadModel(MODEL_DEBRIS, &g_Base[i].object.modelInfo);
		GetModelDiffuse(&g_Base[i].object.modelInfo, g_Base[i].object.modelDiffuse);
		g_Base[i].object.load = true;

		g_Base[i].object.SetPosition(XMFLOAT3{ 500.0f,0.0f,-300.0f });
		g_Base[i].object.SetRotation(XMFLOAT3{ 0.0f,3.1f,0.0f });
		g_Base[i].object.SetScale(XMFLOAT3{ 6.5f,8.5f,6.5f });
		g_Base[i].use = true;
		g_Base[i].size = BASE_SIZE;
	}
	return S_OK;
}

void UninitBase(void)
{
	//���f���̉������
	for (int i = 0; i < MAX_BASE; i++)
	{
		if (g_Base[i].object.load)
		{
			UnloadModel(&g_Base[i].object.modelInfo);
			g_Base[i].object.load = false;
		}
	}
}

void UpdateBase(void)
{
	for (int i = 0; i < MAX_BASE; i++)
	{
		XMFLOAT3 position = g_Base[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Base[i].object.GetRotationFloat();

		if (g_Base[i].use)
		{
		}

		// �ړ���]�𔽉f
		g_Base[i].object.SetPosition(position);
		g_Base[i].object.SetRotation(rotation);

	}
}

void DrawBase(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BASE; i++)
	{
		if (!g_Base[i].use) continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		XMFLOAT3 scale = g_Base[i].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		XMFLOAT3 rotation = g_Base[i].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		XMFLOAT3 position = g_Base[i].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Base[i].object.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_Base[i].object.modelInfo);

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);
	}
}

BASE* GetBase(void) {

	return &g_Base[0];
}

