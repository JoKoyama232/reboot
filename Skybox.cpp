// �w�i���� [Skybox.cpp]
// Author : ���R�@��
//=============================================================================

// �C���N���[�h

// ���g�̃C���N���[�h
#include "Skybox.h"

// �}�N����`
#define MODEL_SKYBOX "data/model/skybox.obj"

// �O���[�o���ϐ�
static Skybox g_Skybox;

// ��������
HRESULT InitSkybox(void) {
	g_Skybox.transform.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	g_Skybox.transform.SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	g_Skybox.transform.SetScale(XMFLOAT3(0.0f, 0.0f, 0.0f));

	g_Skybox.load = true;

	return S_OK;
}

// �I������