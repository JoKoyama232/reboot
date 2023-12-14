// �f�u������ [pod.cpp]
// Author ����l�@��
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "pod.h"
#include "sprite.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_POD		"Data/model/pod(2).obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���

#define TEXTURE_MAX			(1)								// �e�N�X�`���̐�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


static float        g_LastUpdate;
POD g_Pod[MAX_POD];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitPod(void) {

	for (int i = 0; i < MAX_POD; i++)
	{
		LoadModel(MODEL_POD, &g_Pod[i].object.modelInfo);
		GetModelDiffuse(&g_Pod[i].object.modelInfo, g_Pod[i].object.modelDiffuse);
		g_Pod[i].object.load = true;
		g_Pod[i].object.SetPosition(XMFLOAT3{ rand() % 1000 + 0.0f, 0.0f, rand() % 1000 + 0.0f });
		g_Pod[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Pod[i].object.SetScale(XMFLOAT3{ 3.0f, 3.0f, 3.0f });
		g_Pod[i].time = 0.0f;
		g_Pod[i].speed = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Pod[i].use = true;
		g_Pod[i].flag_rotate = true;
		g_Pod[i].object.draw = true;
		g_Pod[i].size = POD_SIZE;
		g_LastUpdate = 0.0f;

	}
	return S_OK;

}

void UninitPod(void) {
	// ���f���̉������
	for (int i = 0; i < MAX_POD; i++)
	{

		if (g_Pod[i].object.load)
		{
			UnloadModel(&g_Pod[i].object.modelInfo);
			g_Pod[i].object.load = false;
		}
	}
}

void UpdatePod(void) {
	// �f�u���ϐ��擾
	for (int i = 0; i < MAX_POD; i++)
	{
		// �f�u���̎g�p�t���O���m�F
		if (!g_Pod[i].use)
		{
			g_Pod[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_Pod[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Pod[i].object.GetRotationFloat();


		if (!g_Pod[i].flag_rotate)continue;
		// ���邮���]
		rotation.x += (i + 1) * 0.005f;
		rotation.y += ((rand() % 5) + 1) * 0.001f;
		rotation.z += ((MAX_POD + 1) - i) * 0.005f;

		// �ړ���]�𔽉f
		g_Pod[i].object.SetPosition(position);
		g_Pod[i].object.SetRotation(rotation);
	}

}

void DrawPod(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_POD; i++)
	{
		if (!g_Pod[i].object.draw) continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		XMFLOAT3 scale = g_Pod[i].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		XMFLOAT3 rotation = g_Pod[i].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		XMFLOAT3 position = g_Pod[i].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_Pod[i].object.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_Pod[i].object.modelInfo);

		SetFuchi(0);

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);

	}
}

POD* GetPod(void) {

	return &g_Pod[0];
}

