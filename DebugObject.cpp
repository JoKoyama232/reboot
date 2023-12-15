//=============================================================================
// �f�o�b�O�I�u�W�F�N�g���� [DebugObj.cpp]
// Author : ���R�@��
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "WorldSpaceClass.h"
#include "debugproc.h"
#include "DebugObject.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_DebugObj_Sphere		"Data/Model/debugDistance.obj"	// �ǂݍ��ރ��f����
#define DebugObj_AMNT_MAX		(3)					// ���P�b�g�̍ő�`�搔+1�@(���[�v�����̈�)
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static DebugObj g_DebugObj[DebugObj_AMNT_MAX];	// ���P�b�g�̔z��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitDebugObj(void)
{
	for (int idx = 0; idx < DebugObj_AMNT_MAX; idx++) {

		LoadModel(MODEL_DebugObj_Sphere, &g_DebugObj[idx].transform.modelInfo);		// ���P�b�g�̃��f���ݒ�
		g_DebugObj[idx].transform.load = false;					// �G���[���Ȃ���΃��f���ǂݍ��ݐ���
		g_DebugObj[idx].transform.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));	// �f�t�H���g�̈ʒu
		g_DebugObj[idx].transform.SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));	// �f�t�H���g�̉�]
		g_DebugObj[idx].transform.SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));		// �f�t�H���g�̃X�P�[��
	}
	// �S�Ẵ��P�b�g���̏������l���ݒ肳�ꂽ���ߏ���������
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitDebugObj(void)
{
	for (int idx = 0; idx < DebugObj_AMNT_MAX; idx++) {
		// ���f���̉������
		if (g_DebugObj[idx].transform.load == true)
		{
			UnloadModel(&g_DebugObj[idx].transform.modelInfo);
			g_DebugObj[idx].transform.load = false;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDebugObj(void)
{
	// �X�V�͊e���̑Ώۂ̃N���X�ōs��
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDebugObj(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix, mtxObj;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int idx = 0; idx < DebugObj_AMNT_MAX; idx++) {
		// �g�p����Ă��Ȃ����P�b�g�͕`����s��Ȃ�
		if (!g_DebugObj[idx].use) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		//���͏��ɃX�P�[�����Q�b�g
		XMFLOAT3 input = g_DebugObj[idx].transform.GetScaleFloat();
		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		//���͏��ɉ�]���Q�b�g
		input = g_DebugObj[idx].transform.GetRotationFloat();
		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


		//���͏��Ɉړ����Q�b�g
		input = g_DebugObj[idx].transform.GetPositionFloat();
		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(input.x, input.y, input.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		// ���[���h�}�g���b�N�X�̕ۑ�
		XMStoreFloat4x4(g_DebugObj[idx].transform.GetWorldMatrixPointer(), mtxWorld);

		// ���f���`��
		DrawModel(&g_DebugObj[idx].transform.modelInfo);
	}
	XMFLOAT3 position = g_DebugObj[0].transform.GetPositionFloat();
	XMFLOAT3 rotation = g_DebugObj[0].transform.GetScaleFloat();
	PrintDebugProc((char*)"Debug Information\nPosition:(%f, %f, %f)\nScale:(%f, %f, %f)\n", position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);

	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �f�o�b�O�I�u�W�F�N�g����
//=============================================================================
int SetDebugObj(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
{
	for (int idx = 0; idx < DebugObj_AMNT_MAX; idx++) 
	{

		// �ŏ��̃I�u�W�F�N�g���g�p����Ă��Ȃ���΃��[�v
		if (idx == DebugObj_AMNT_MAX - 1 && !g_DebugObj[0].use) {
			idx = 0;
		}

		// �g�p����Ă���Ύ��֍s��
		if (g_DebugObj[idx].use) continue;

		g_DebugObj[idx].transform.SetPosition(position);
		g_DebugObj[idx].transform.SetRotation(rotation);
		g_DebugObj[idx].transform.SetScale(scale);
		g_DebugObj[idx].use = true;
		return idx;
	}
	// ���s����-1��߂�
	return -1;
}

//=============================================================================
// �f�o�b�O�I�u�W�F�N�g�̏����擾
//=============================================================================
DebugObj* GetDebugObj(int idx)
{
	return &g_DebugObj[idx];
}