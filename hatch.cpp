// �f�u������ [hatch.cpp]
// Author ����l�@��
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "hatch.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_HATCH		"Data/model/hatch.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���

#define TEXTURE_MAX			(1)								// �e�N�X�`���̐�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


static float        g_LastUpdate;
HATCH g_Hatch[MAX_HATCH];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitHatch(void) {

	for (int i = 0; i < MAX_HATCH; i++)
	{
		LoadModel(MODEL_HATCH, &g_Hatch[i].object.modelInfo);
		GetModelDiffuse(&g_Hatch[i].object.modelInfo, g_Hatch[i].object.modelDiffuse);
		g_Hatch[i].object.load = true;
		g_Hatch[i].object.SetPosition(XMFLOAT3{ rand() % 1000 + 0.0f, rand() % 1000 + 0.0f, rand() % 1000 + 0.0f });
		g_Hatch[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Hatch[i].object.SetScale(XMFLOAT3{ 0.6f, 0.6f, 0.6f });
		g_Hatch[i].time = 0.0f;
		g_Hatch[i].speed = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Hatch[i].use = true;
		g_Hatch[i].flag_rotate = true;
		g_Hatch[i].object.draw = true;
		g_Hatch[i].size = HATCH_SIZE;
		g_LastUpdate = 0.0f;

	}
	return S_OK;

}

void UninitHatch(void) {
	// ���f���̉������
	for (int i = 0; i < MAX_HATCH; i++)
	{

		if (g_Hatch[i].object.load)
		{
			UnloadModel(&g_Hatch[i].object.modelInfo);
			g_Hatch[i].object.load = false;
		}
	}
}

void UpdateHatch(void) {
	// �f�u���ϐ��擾
	for (int i = 0; i < MAX_HATCH; i++)
	{
		// �f�u���̎g�p�t���O���m�F
		if (!g_Hatch[i].use)
		{
			g_Hatch[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_Hatch[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Hatch[i].object.GetRotationFloat();

		if (!g_Hatch[i].flag_rotate)continue;
		{
			// ��]������
			rotation.x += (i + 1) * 0.005f;
			rotation.y += ((rand() % 5) + 1) * 0.001f;
			rotation.z += ((MAX_HATCH + 1) - i) * 0.005f;
		}

		// �ړ���]�𔽉f
		g_Hatch[i].object.SetPosition(position);
		g_Hatch[i].object.SetRotation(rotation);
	}

}

void DrawHatch(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_HATCH; i++)
	{
		if (!g_Hatch[i].object.draw) continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		XMFLOAT3 scale = g_Hatch[i].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		XMFLOAT3 rotation = g_Hatch[i].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		XMFLOAT3 position = g_Hatch[i].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Hatch[i].object.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_Hatch[i].object.modelInfo);

		SetFuchi(0);

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);

	}
}

HATCH* GetHatch(void) {

	return &g_Hatch[0];
}

