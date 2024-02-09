//=============================================================================
//
// ���_�Ɍ�����̏��� [basearoow.cpp]
// Author : ��c �P
//
//=============================================================================

#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "fade.h"
#include "base.h"
#include "basearrow.h"
#include "Player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ARROW		"Data/model/arrow.obj"			// �ǂݍ��ރ��f����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static float        g_LastUpdate;
ARROW g_Arrow;
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���

HRESULT InitArrow(void)
{
	LoadModel(MODEL_ARROW, &g_Arrow.object.modelInfo);
	GetModelDiffuse(&g_Arrow.object.modelInfo, g_Arrow.object.modelDiffuse);
	g_Arrow.object.load = true;

	XMFLOAT3 playerPos;
	playerPos = GetPlayer()->object.GetPositionFloat();

	g_Arrow.object.SetPosition(XMFLOAT3{ playerPos.x,playerPos.y + 100.0f,playerPos.z });
	g_Arrow.object.SetRotation(XMFLOAT3{ 0.0f,0.0f,0.0f });
	g_Arrow.object.SetScale(XMFLOAT3{ 1.0f,1.0f,1.0f });
	g_Arrow.at = { 0.0f, 0.0f, 0.0f };

	g_Arrow.use = true;
	g_Arrow.size = ARROW_SIZE;

	// ���_�ƒ����_�̋������v�Z
	XMFLOAT3 arrowPos = GetArrow()->object.GetPositionFloat();
	float vx, vz;
	vx = arrowPos.x - g_Arrow.at.x;
	vz = arrowPos.z - g_Arrow.at.z;
	g_Arrow.len = sqrtf(vx * vx + vz * vz);

	return S_OK;
}

void UninitArrow(void)
{
	//���f���̉������
	if (g_Arrow.object.load)
	{
		UnloadModel(&g_Arrow.object.modelInfo);
		g_Arrow.object.load = false;
	}
}

void UpdateArrow(void)
{
	XMFLOAT3 position = g_Arrow.object.GetPositionFloat();
	XMFLOAT3 rotation = g_Arrow.object.GetRotationFloat();

	XMFLOAT3 playerPos = GetPlayer()->object.GetPositionFloat();
	XMFLOAT3 basePos = GetBase()->object.GetPositionFloat();

	if (g_Arrow.use)
	{
		// �v���C���[�̓���ɕ\��
		position.x = playerPos.x;
		position.y = playerPos.y + 50.0f;
		position.z = playerPos.z;

		rotation.x = atan2f(basePos.y - playerPos.y, sqrtf((basePos.x - playerPos.x) * (basePos.x - playerPos.x) + (basePos.z - playerPos.z) * (basePos.z - playerPos.z)));
		rotation.y = atan2f(basePos.x - playerPos.x, basePos.z - playerPos.z);
	}

	//���W�̃Z�b�g
	g_Arrow.object.SetPosition(position);
	g_Arrow.object.SetRotation(rotation);

}

void DrawArrow(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	if (g_Arrow.use)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		XMFLOAT3 scale = g_Arrow.object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		XMFLOAT3 rotation = g_Arrow.object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		XMFLOAT3 position = g_Arrow.object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Arrow.object.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_Arrow.object.modelInfo);

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);

	}
}

ARROW* GetArrow(void)
{
	return &g_Arrow;
}