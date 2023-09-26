// �w�i���� [Skybox.h]
// Author : ���R�@��
//=============================================================================
#pragma once
// �C���N���[�h
#include "model.h"
#include "WorldSpaceClass.h"


// �\����`
struct Skybox {
	// ���[���h�ʒu���
	WorldSpace transform;

	// �ǂݍ��ݏ󋵊Ǘ�
	bool use = false;
	bool load = false;

};

// �֐��Q
HRESULT InitSkybox(void);
void UninitSkybox(void);
void UpdatSkybox(void);
void DrawSkybox(void);