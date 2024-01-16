// �w�i���� [Skybox.cpp]
// Author : ���R�@��
//=============================================================================

// �C���N���[�h
#include "model.h"
#include "WorldSpaceClass.h"

// ���g�̃C���N���[�h
#include "Skybox.h"

// �}�N����`
enum sky {
	SkyBG = 0,
	Earth,
	Clouds,
	SkyObjectMax
};
		
// �O���[�o���ϐ�
static WorldSpace g_Sky[3];		// ��̃I�u�W�F�N�g�i�Ǝ��ϐ�������Ȃ����߃��f���N���X���g�p�j

static char* skyModelAdr[3]{	// ��̃I�u�W�F�N�g�̃��f���A�h���X
	(char*)"Data/Model/Skysphere_blue.obj",	// ��̔w�i����
	(char*)"Data/Model/earth_half.obj",	// �n��
	(char*)"Data/Model/Cloud.obj"		// ���ɉ�]���Ă���_
};

// ��������
HRESULT InitSkybox(void) {
	for (int idx = 0; idx < SkyObjectMax; idx++) {
		// ���f���̏�����
		LoadModel(skyModelAdr[idx], &g_Sky[idx].modelInfo);
		GetModelDiffuse(&g_Sky[idx].modelInfo, g_Sky[idx].modelDiffuse);
		g_Sky[idx].load = true;		
		// ���f���̏���������

		// �ϐ�������
		g_Sky[idx].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_Sky[idx].SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_Sky[idx].SetScale(XMFLOAT3(15.0f, 15.0f, 15.0f));

	}

	g_Sky[Earth].SetPosition(XMFLOAT3(0.0f, -4000.0f, 0.0f));
	g_Sky[Earth].SetScale(XMFLOAT3(40.0f, 40.0f, 40.0f));
	g_Sky[SkyBG].SetScale(XMFLOAT3(15.0f, 15.0f, 15.0f));
	g_Sky[Clouds].SetPosition(XMFLOAT3(0.0f, -1600.0f, 0.0f));
	g_Sky[Clouds].SetScale(XMFLOAT3(130.0f, 130.0f, 130.0f));
	return S_OK;
}

// �I������
void UninitSkybox(void) {
	for (int idx = 0; idx < SkyObjectMax; idx++) {
		if (!g_Sky[idx].load) continue;

		UnloadModel(&g_Sky[idx].modelInfo);
		g_Sky[idx].load = false;
		
	}
}

void UpdateSkybox(void) {
	// ��̉�]����
	XMFLOAT3 rotation = g_Sky[SkyBG].GetRotationFloat();
	rotation.y += XM_PI * 0.0005f;
	if (rotation.y > XM_PI * 2.0f) {
		rotation.y - XM_PI * 2.0f;
	}
	g_Sky[SkyBG].SetRotation(rotation);

	// �ϐ�rotation�͉_�̔w�i��]�ɍĒ�`
	rotation = g_Sky[Earth].GetRotationFloat();
	rotation.y += XM_PI * 0.0007f;
	if (rotation.y > XM_PI * 2.0f) {
		rotation.y - XM_PI * 2.0f;
	}
	g_Sky[Earth].SetRotation(rotation);

	// �ϐ�rotation�͉_�̔w�i��]�ɍĒ�`
	rotation = g_Sky[Clouds].GetRotationFloat();
	rotation.y += XM_PI * 0.001f;
	if (rotation.y > XM_PI * 2.0f) {
		rotation.y - XM_PI * 2.0f;
	}
	g_Sky[Clouds].SetRotation(rotation);
}

void DrawSkybox(void) {
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// BACK�J�����O�@�i���̂̒��ɃJ���������邽�߁j
	SetCullingMode(CULL_MODE_BACK);

	// ���𖳌�
	SetLightEnable(false);

	for (int idx = 0; idx < SkyObjectMax; idx++) {
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		//���͏��ɃX�P�[�����Q�b�g
		XMFLOAT3 input = g_Sky[idx].GetScaleFloat();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		//���͏��ɉ�]���Q�b�g
		input = g_Sky[idx].GetRotationFloat();

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		//���͏��Ɉړ����Q�b�g
		input = g_Sky[idx].GetPositionFloat();

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		// ���[���h�}�g���b�N�X�̕ۑ�
		XMStoreFloat4x4(g_Sky[idx].GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_Sky[idx].modelInfo);
	}

	// ����L���L���ɂ���
	SetLightEnable(true);

	// �J�����O��BACK�ɖ߂�
	SetCullingMode(CULL_MODE_BACK);
}