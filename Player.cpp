// �v���C���[���� [Player.cpp]
// Author : ���R�@��@����l�@��
//=============================================================================
#include "model.h"
#include "main.h"
#include "camera.h"

#include "input.h"
#include "debugproc.h"
#include "renderer.h"
#include "bullet.h"

#include "Player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER		"Data/model/cone.obj"			// �ǂݍ��ރ��f����(�܂����݂��ĂȂ���)

#define	VALUE_MOVE			(2.0f)							// �ړ���

static float		roty = 0.0f;


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static float        g_LastUpdate;
PLAYER g_Player;
static bool g_fire = false;

HRESULT InitPlayer(void) {

		LoadModel(MODEL_PLAYER, &g_Player.object.modelInfo);
		GetModelDiffuse(&g_Player.object.modelInfo, g_Player.object.modelDiffuse);
		g_Player.object.load = true;

		g_Player.object.SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Player.object.SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
		g_Player.object.SetScale(XMFLOAT3{ 1.0f, 1.0f, 1.0f });
		g_Player.time = 0.0f;
		g_Player.speed = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Player.use = true;
		g_LastUpdate = 0.0f;
		roty = 0.0f;

		return S_OK;

}

void UninitPlayer(void) {
	// ���f���̉������
	if (g_Player.object.load)
	{
		UnloadModel(&g_Player.object.modelInfo);
		g_Player.object.load = false;
	}

}

void UpdatePlayer(void) {

	// �v���C���[�ϐ��擾
	XMFLOAT3 position = g_Player.object.GetPositionFloat();
	XMFLOAT3 rotation = g_Player.object.GetRotationFloat();

	// �t�@�C���O�ϐ��̎擾
	DWORD updateTime = timeGetTime();
	DWORD deltaTime = updateTime - g_LastUpdate;
	g_LastUpdate = updateTime;
	CAMERA* cam = GetCamera();
	XMFLOAT3 camPosition = cam->pos;
	XMFLOAT3 camRotation = cam->rot;

	// ���͌��m
	if (GetKeyboardPress(DIK_A) || IsButtonTriggered(0, BUTTON_LEFT))
	{	// ��
		g_Player.speed = 1.0f;
		g_Player.direction = -XM_PI / 2;
	}
	else if (GetKeyboardPress(DIK_D) || IsButtonTriggered(0, BUTTON_RIGHT))
	{	// �E
		g_Player.speed = 1.0f;
		g_Player.direction = XM_PI / 2;
	}
	else {
		g_Player.direction = 0.0f;
	}

	if (GetKeyboardPress(DIK_W) || IsButtonTriggered(0, BUTTON_UP))
	{	// �O
		g_Player.speed = 1.0f;
		g_Player.direction = 0.0f;
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN))
	{	// ���
		g_Player.speed = 1.0f;
		g_Player.direction = XM_PI;
	}

	if ((GetKeyboardPress(DIK_SPACE)) || IsMouseLeftTriggered())
	{
		g_Player.object.SetPosition(XMFLOAT3{ 1.0f, 0.0f, 0.0f });
		g_Player.speed = VALUE_MOVE;
		roty = XM_PI;
	}
	//else if (IsButtonTriggered(0, BUTTON_UP))
	//{
	//	g_Player.spd = VALUE_MOVE;
	//	roty = XM_PI;
	//}
	else
	{
		g_Player.object.SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
	}

	// �ړ�����
	// XZ���ʈړ�
	// �v���C���[�̌����Ă���������J��������I�t�Z�b�g
	rotation.y = g_Player.direction + camRotation.y;

	// ���͂̂����������փv���C���[���������Ĉړ�������
	// �v���C���[�̌����Ă��������ۑ�
	float deltaX = sinf(rotation.y) * g_Player.speed * deltaTime;
	float deltaZ = cosf(rotation.y) * g_Player.speed * deltaTime;
	g_Player.speed *= 0.2f;
	//�ړ��𔽉f
	position.x += deltaX;
	position.z += deltaZ;

	// ���x�iY���ړ��j
	float deltaY = 0.0f * deltaTime;
	position.y += deltaY;

	// �ړ��𔽉f(����ȏ�v���C���[�̈ʒu��ς��Ȃ�)
	g_Player.object.SetPosition(position);
	g_Player.object.SetRotation(rotation);

	// �e���ˏ���
	if (GetKeyboardTrigger(DIK_SPACE)/* && updateTime - g_LastUpdate > 3000*/)
	{
 		SetBullet(position, rotation);
		//g_LastUpdate = updateTime;
		
	}
	PrintDebugProc((char*)"Player Information\nMovement:   W\n            A  S  D\n  Shift    Space\nPosition:(%f, %f, %f)\nRotation:(%f, %f, %f)\n", position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);

}

void DrawPlayer(void) {

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	XMFLOAT3 scale = g_Player.object.GetScaleFloat();
	mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	XMFLOAT3 rotation = g_Player.object.GetRotationFloat();
	mtxRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);


	// �ړ��𔽉f
	XMFLOAT3 position = g_Player.object.GetPositionFloat();
	mtxTranslate = XMMatrixTranslation(position.x, position.y, position.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	
	XMStoreFloat4x4(g_Player.object.GetWorldMatrixPointer(), mtxWorld);


	// �����̐ݒ�
	SetFuchi(1);

	// ���f���`��
	DrawModel(&g_Player.object.modelInfo);

	SetFuchi(0);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

}

PLAYER* GetPlayer(void) {
	return &g_Player;
}

XMFLOAT3 GetLerp(XMFLOAT3 initialPos, XMFLOAT3 endingPos, float percentage) {
	XMFLOAT3 result;
	XMVECTOR pos0 = XMLoadFloat3(&initialPos);
	XMVECTOR pos1 = XMLoadFloat3(&endingPos);

	pos0 += (pos1 - pos0) * percentage;
	XMStoreFloat3(&result, pos0);
	return result;
}

float GetDistance3D(XMFLOAT3 pos1, XMFLOAT3 pos2) {
	return (float)sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y) + (pos1.z - pos2.z) * (pos1.z - pos2.z));
}