//=============================================================================
// ���[���h�X�y�[�X�N���X [WorldSpaceClass.h]
// Author : Jo Koyama
//=============================================================================
#pragma once
#include "main.h"


class WorldSpace {
	XMFLOAT4X4 worldMtx = XMFLOAT4X4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	WorldSpace* parent = NULL;
public:
	//�Z�b�^�[
	void SetWorldMatrix(XMFLOAT4X4 a_worldMtx) { worldMtx = a_worldMtx; }
	void SetPosition(XMFLOAT3 a_position) {
		position = a_position;
	}
	void SetPosition(XMVECTOR a_position) {
		XMStoreFloat3(&position, a_position);
	}
	void SetRotation(XMFLOAT3 a_rotation) {
		rotation = a_rotation;
	}
	void SetRotation(XMVECTOR a_rotation) {
		XMStoreFloat3(&rotation, a_rotation);
	}
	void SetScale(XMFLOAT3 a_scale) {
		scale = a_scale;
	}
	void SetScale(XMVECTOR a_scale) {
		XMStoreFloat3(&scale, a_scale);
	}
	void SetParent(WorldSpace* a_parent) { parent = a_parent; }

	//�Q�b�^�[
	XMFLOAT4X4 GetWorldMatrix(void) { return worldMtx; }
	XMFLOAT3 GetPositionFloat(void) { return position; }
	XMVECTOR GetPositionVector(void) { return XMLoadFloat3(&position); }
	XMFLOAT3 GetRotationFloat(void) { return rotation; }
	XMVECTOR GetRotationVector(void) { return XMLoadFloat3(&rotation); }
	XMFLOAT3 GetScaleFloat(void) { return scale; }
	XMVECTOR GetScaleVector(void) { return XMLoadFloat3(&scale); }
	WorldSpace* GetParent(void) {
		if(parent != NULL){ return parent; }
		 }
};
