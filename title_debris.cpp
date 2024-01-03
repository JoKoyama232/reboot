// �f�u������ [debris.cpp]
// Author ��c
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "title_debris.h"
#include "sprite.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_DEBRIS		"Data/model/debris.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���

#define TEXTURE_MAX			(1)								// �e�N�X�`���̐�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


static float        g_LastUpdate;
TDEBRIS g_TDebris[MAX_DEBRIS];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitTitleDebris(void) {

	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		LoadModel(MODEL_DEBRIS, &g_TDebris[i].object.modelInfo);
		GetModelDiffuse(&g_TDebris[i].object.modelInfo, g_TDebris[i].object.modelDiffuse);
		g_TDebris[i].object.load = true;
		g_TDebris[i].object.SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_TDebris[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_TDebris[i].object.SetScale(XMFLOAT3{ 0.1f, 0.1f, 0.1f });
		g_TDebris[i].time = 0.0f;
		g_TDebris[i].speed = 0.0f;			// �ړ��X�s�[�h�N���A
		g_TDebris[i].use = true;
		g_TDebris[i].flag_rotate = true;
		g_TDebris[i].object.draw = true;
		g_TDebris[i].size = DEBRIS_SIZE;
		g_LastUpdate = 0.0f;


		//�^�C�g�������ʒu�̎w��
		g_TDebris[0].object.SetPosition(XMFLOAT3{ -100.0f,   0.0f, 1000.0f });
		g_TDebris[1].object.SetPosition(XMFLOAT3{  100.0f, -50.0f,  300.0f });
		g_TDebris[2].object.SetPosition(XMFLOAT3{ -200.0f,  20.0f,  200.0f });
		g_TDebris[3].object.SetPosition(XMFLOAT3{ -150.0f,-200.0f,  300.0f });
		g_TDebris[4].object.SetPosition(XMFLOAT3{  150.0f,-100.0f,  100.0f });
		g_TDebris[5].object.SetPosition(XMFLOAT3{  150.0f,  10.0f,  100.0f });
	}
	return S_OK;

}

void UninitTitleDebris(void) {
	// ���f���̉������
	for (int i = 0; i < MAX_DEBRIS; i++)
	{

		if (g_TDebris[i].object.load)
		{
			UnloadModel(&g_TDebris[i].object.modelInfo);
			g_TDebris[i].object.load = false;
		}
	}
}

void UpdateTitleDebris(void) {
	// �f�u���ϐ��擾
	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		// �f�u���̎g�p�t���O���m�F
		if (!g_TDebris[i].use)
		{
			g_TDebris[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_TDebris[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_TDebris[i].object.GetRotationFloat();

		if (!g_TDebris[i].flag_rotate)continue;
		{
			// ���邮���]
			rotation.x += (i + 1) * 0.001f;
			rotation.y += ((rand() % 5) + 1) * 0.001f;
			rotation.z += ((MAX_DEBRIS + 1) - i) * 0.001f;
		}

		// �ړ���]�𔽉f
		g_TDebris[i].object.SetPosition(position);
		g_TDebris[i].object.SetRotation(rotation);
	}

}

void DrawTitleDebris(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		if (!g_TDebris[i].object.draw) continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		XMFLOAT3 scale = g_TDebris[i].object.GetScaleFloat();
		mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		XMFLOAT3 rotation = g_TDebris[i].object.GetRotationFloat();
		mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		XMFLOAT3 position = g_TDebris[i].object.GetPositionFloat();
		mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(g_TDebris[i].object.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_TDebris[i].object.modelInfo);

		SetFuchi(0);

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);

	}
}

TDEBRIS* GetTitleDebris(void) {

	return &g_TDebris[0];
}

