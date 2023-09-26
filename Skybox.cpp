// �w�i���� [Skybox.cpp]
// Author : ���R�@��
//=============================================================================

// �C���N���[�h
#include "model.h"
// ���g�̃C���N���[�h
#include "Skybox.h"

// �}�N����`
#define MODEL_SKYBOX "Data/model/Skybox.obj"

// �O���[�o���ϐ�
static Skybox g_Skybox;

// ��������
HRESULT InitSkybox(void) {
	// ���f���̏�����
	LoadModel(MODEL_SKYBOX, &g_Skybox.transform.modelInfo);
	GetModelDiffuse(&g_Skybox.transform.modelInfo, g_Skybox.transform.modelDiffuse);
	g_Skybox.transform.load = true;		// ���f���̏���������

	g_Skybox.transform.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	g_Skybox.transform.SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	g_Skybox.transform.SetScale(XMFLOAT3(0.0f, 0.0f, 0.0f));

	g_Skybox.load = true;

	return S_OK;
}

// �I������
void UninitSkybox(void) {
	if (g_Skybox.transform.load) {
		UnloadModel(&g_Skybox.transform.modelInfo);
		g_Skybox.transform.load = false;
	}
}

void UpdateSkybox(void) {

}

void DrawSkybox(void) {

}