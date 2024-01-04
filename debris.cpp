// �f�u������ [debris.cpp]
// Author ����l�@��
//=============================================================================
#include "model.h"
#include "main.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "debris.h"
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
DEBRIS g_Debris[MAX_DEBRIS];
static bool g_fire = false;
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",

};

HRESULT InitDebris(void) {

	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		LoadModel(MODEL_DEBRIS, &g_Debris[i].object.modelInfo);
		GetModelDiffuse(&g_Debris[i].object.modelInfo, g_Debris[i].object.modelDiffuse);
		g_Debris[i].object.load = true;
		g_Debris[i].object.SetPosition(XMFLOAT3{ rand() % 1000 + 0.0f, rand() % 1000 - 500.0f, rand() % 1000 + 0.0f });
		g_Debris[i].object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Debris[i].object.SetScale(XMFLOAT3{ 0.1f, 0.1f, 0.1f });
		g_Debris[i].time = 0.0f;
		g_Debris[i].speed = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Debris[i].use = true;
		g_Debris[i].flag_rotate = true;
		g_Debris[i].object.draw = true;
		g_Debris[i].size = DEBRIS_SIZE;
		g_LastUpdate = 0.0f;

	}
		return S_OK;
	
}

void UninitDebris(void) {
	// ���f���̉������
	for (int i = 0; i < MAX_DEBRIS; i++)
	{

		if (g_Debris[i].object.load)
		{
			UnloadModel(&g_Debris[i].object.modelInfo);
			g_Debris[i].object.load = false;
		}
	}
}

void UpdateDebris(void) {
	// �f�u���ϐ��擾
	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		// �f�u���̎g�p�t���O���m�F
		if (!g_Debris[i].use)
		{
			g_Debris[i].object.draw = false;
			continue;
		}

		XMFLOAT3 position = g_Debris[i].object.GetPositionFloat();
		XMFLOAT3 rotation = g_Debris[i].object.GetRotationFloat();

		if (!g_Debris[i].flag_rotate)continue;
		{
			// ���邮���]
			rotation.x += (i + 1) * 0.001f;
			rotation.y += ((rand() % 5) + 1) * 0.001f;
			rotation.z += ((MAX_DEBRIS + 1) - i) * 0.001f;
		}

		// �ړ���]�𔽉f
		g_Debris[i].object.SetPosition(position);
		g_Debris[i].object.SetRotation(rotation);
	}

}

void DrawDebris(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		if (!g_Debris[i].object.draw) continue;
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	XMFLOAT3 scale = g_Debris[i].object.GetScaleFloat();
	mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	XMFLOAT3 rotation = g_Debris[i].object.GetRotationFloat();
	mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	XMFLOAT3 position = g_Debris[i].object.GetPositionFloat();
	mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(g_Debris[i].object.GetWorldMatrixPointer(), mtxWorld);

	// ���f���`��
	DrawModel(&g_Debris[i].object.modelInfo);

	SetFuchi(0);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

	}
}

DEBRIS* GetDebris(void) {
	
	return &g_Debris[0];
}

