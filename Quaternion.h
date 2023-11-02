#pragma once

class Quaternion {
	// �N�I�[�^�j�I���\�L
	XMFLOAT4 quaternion = XMFLOAT4{ 0.0f,0.0f,0.0f,1.0f};

	// AA(AngleAxis)�\�L
	// �N�I�[�^�j�I���̉�]����\��
	XMFLOAT3 axis = XMFLOAT3{ 0.0f,0.0f,0.0f};
	//�N�I�[�^�j�I���̊p�x���w��
	float angle = 0.0f;
public:
	// �X�V����(���������Q�ʂ�̕\�L�ŕ\�����߁A�Z�b�g�֐��̌�X�V�֐����Ă�)
	void UpdateAA(void);			// AA�\�L���X�V
	void UpdateQuaternion(void);	// �N�I�[�^�j�I���\�L�̍X�V

	// �Q�b�g�֐��Q
	// �N�I�[�^�j�I���\�L��XMFLOAT3�l�ŏo��
	XMFLOAT4 GetQuaternion(void) { 
		return quaternion; 
	}	 
	// �N�I�[�^�j�I���\�L��XMVECTOR�l�ŏo��
	XMVECTOR GetQuaternionV(void) {
		return XMLoadFloat4(&quaternion);
	}	 
	// �N�I�[�^�j�I���\�L��XMFLOAT3�l�ŏo��
	XMFLOAT4 GetQuaternionInv(void) {
		XMFLOAT4 q = GetQuaternion();
		q.x *= -1.0f;
		q.y *= -1.0f;
		q.z *= -1.0f;
		return q;
	}
	// �N�I�[�^�j�I���\�L��XMVECTOR�l�ŏo��
	XMVECTOR GetQuaternionInvV(void) {
		return XMLoadFloat4(&quaternion);
	}
	// ��]����XMFLOAT3�l�ŏo��
	XMFLOAT3 GetVectorF(void) {
		return axis;
	}		
	// ��]����XMVECTOR�l�ŏo��
	XMVECTOR GetVectorV(void) {
		return XMLoadFloat3(&axis); }
	// �p�x���o��
	float GetAngle(void) {
		return angle;
	}
	// �N�I�[�^�j�I�����I�C���[�l�ŏo��
	XMFLOAT3 ConvertToEuler(void);
	XMFLOAT3* ConvertToEulerp(void);
	
	// �Z�b�g�֐��Q
	// �N�I�[�^�j�I���\�L�̓��́iXMVECTOR,XMFLOAT3�Ή��j
	void SetQuaternion(XMFLOAT4 a_Quaternion) {
		quaternion = a_Quaternion;
		UpdateAA();
	}
	void SetQuaternion(XMVECTOR a_Quaternion) { 
		XMStoreFloat4(&quaternion, a_Quaternion);
		UpdateAA();
	}
	// ��]���̓��́iXMVECTOR,XMFLOAT3�Ή��j
	void SetVectorF(XMFLOAT3 vector) {
		axis.x = vector.x;
		axis.y = vector.y;
		axis.z = vector.z;
		UpdateQuaternion();
	}
	void SetVectorV(XMVECTOR vector) {
		XMFLOAT3 store;
		XMStoreFloat3(&store, vector);
		SetVectorF(store);
		UpdateQuaternion();
	}
	// �p�x�̓���
	void SetAngle(float a_angle) {
		/*if (a_angle > 1.0f) { a_angle -= 1.0f; }
		else if (a_angle < -1.0f) { a_angle += 1.0f; }*/
		angle = a_angle;
		UpdateQuaternion();
	}

	
};

XMFLOAT3 RotateVectorFloatByQuaternion(XMFLOAT3 vector, Quaternion quaternion);
XMVECTOR RotateVectorByQuaternion(XMVECTOR vector, Quaternion quaternion);
float Dot(XMVECTOR* vectorDA, XMVECTOR* vectorCB);			// ���όv�Z
bool IsSameDir(XMVECTOR* vectorDA, XMVECTOR* vectorCB);		// �Q�̃x�N�g�������������������Ă邩�H
XMVECTOR Cross(XMVECTOR* vectorDA, XMVECTOR* vectorCB);		// �O�όv�Z�i*���Ԃɒ���*�j
XMFLOAT4 EulerToQuaternion(XMFLOAT3 euler);					// �I�C���[�l���N�I�[�^�j�I���\�L�ɕϊ�
XMFLOAT3 HamiltonVQ(XMFLOAT3 vector, Quaternion quaternion);
XMFLOAT4 HamiltonQQ(XMFLOAT4 q, XMFLOAT4 q2);
float GetDistance(XMFLOAT3 vector);
float faceAngle(XMFLOAT3 vector);