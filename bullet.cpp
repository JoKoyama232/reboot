// �e�ۏ��� [Player.cpp]
// Author 
//=============================================================================
#include "model.h"
#include "main.h"

#include "Player.h"
#include "bullet.h"
#include "input.h"
#include "debugproc.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BULLET		"Data/model/cone.obj"			// �ǂݍ��ރ��f����
															// ���̓��`���Ȃ��̂ŉ�

#define	VALUE_MOVE			(3.0f)							// �ړ���

static float		roty = 0.0f;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

BULLET g_Bullet[MAX_BULLET];


//����������
HRESULT InitBullet(void)
{
	for (int b = 0; b < MAX_BULLET; b++)
	{
		LoadModel(MODEL_BULLET, &g_Bullet[b].object.modelInfo);
		GetModelDiffuse(&g_Bullet[b].object.modelInfo, g_Bullet[b].object.modelDiffuse);
		g_Bullet[b].object.load = true;

		g_Bullet[b].object.SetPosition(XMFLOAT3{ 0.0f,0.0f,0.0f });
		g_Bullet[b].object.SetRotation(XMFLOAT3{ 0.0f,0.0f,0.0f });
		g_Bullet[b].object.SetScale(XMFLOAT3{ 1.0f,1.0f,1.0f });
		g_Bullet[b].time = 0.0f;
		g_Bullet[b].spd = 2.0f;
		g_Bullet[b].use = false;


	}
	return S_OK;
}

//�I������
void UninitBullet(void)
{
	//���f���̉������
	for (int b = 0; b < MAX_BULLET; b++)
	{
		if (g_Bullet[b].object.load)
		{
			UnloadModel(&g_Bullet[b].object.modelInfo);
			g_Bullet[b].object.load = false;
		}
	}
}

//�X�V����
void UpdateBullet(void)
{
	for (int b = 0; b < MAX_BULLET; b++)
	{
		if (!g_Bullet[b].use)continue;
		{
			//PLAYER* player = GetPlayer();
			//XMFLOAT3 p_pos = player->object.GetPositionFloat();
			XMFLOAT3 b_pos = g_Bullet[b].object.GetPositionFloat();
			XMFLOAT3 b_rot = g_Bullet[b].object.GetRotationFloat();

			// �e�̈ړ�����
			b_pos.x += sinf(b_rot.y) * g_Bullet[b].spd;
			b_pos.z += cosf(b_rot.y) * g_Bullet[b].spd;

			g_Bullet[b].object.SetPosition(XMFLOAT3{ b_pos.x,0.0f,b_pos.z });
		}
	}
}

//�`�揈��
void DrawBullet(void)
{

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	for (int b = 0; b < MAX_BULLET; b++)
	{
		if (g_Bullet[b].use)
		{
			// �X�P�[���𔽉f
			XMFLOAT3 scale = g_Bullet[b].object.GetScaleFloat();
			mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			XMFLOAT3 rotation = g_Bullet[b].object.GetRotationFloat();
			mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			XMFLOAT3 position = g_Bullet[b].object.GetPositionFloat();
			mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);


			XMStoreFloat4x4(g_Bullet[b].object.GetWorldMatrixPointer(), mtxWorld);


			// �����̐ݒ�
			SetFuchi(1);

			// ���f���`��
			DrawModel(&g_Bullet[b].object.modelInfo);

			SetFuchi(0);

			// �J�����O�ݒ��߂�
			SetCullingMode(CULL_MODE_BACK);
		}
	}
}

BULLET* GetBullet(void) 
{
	return &(g_Bullet[0]);
}

//=============================================================================
// �e�̃p�����[�^���Z�b�g
//=============================================================================
int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot)
{
	int nIdxBullet = -1;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_Bullet[nCntBullet].use)continue;

		g_Bullet[nCntBullet].object.SetPosition(pos);
		g_Bullet[nCntBullet].object.SetRotation(rot);
		g_Bullet[nCntBullet].object.SetScale({XMFLOAT3(1.0f,1.0f,1.0f)});
		g_Bullet[nCntBullet].use = true;

		nIdxBullet = nCntBullet;

		//PlaySound(SOUND_LABEL_SE_shot000);

			
		return nIdxBullet;

	}
	return nIdxBullet;

}