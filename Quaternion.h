#pragma once

class Quaternion {
	// クオータニオン表記
	XMFLOAT4 quaternion = XMFLOAT4{ 0.0f,0.0f,0.0f,1.0f};

	// AA(AngleAxis)表記
	// クオータニオンの回転軸を表す
	XMFLOAT3 axis = XMFLOAT3{ 0.0f,0.0f,0.0f};
	//クオータニオンの角度を指す
	float angle = 0.0f;
public:
	// 更新処理(同じ情報を２通りの表記で表すため、セット関数の後更新関数を呼ぶ)
	void UpdateAA(void);			// AA表記を更新
	void UpdateQuaternion(void);	// クオータニオン表記の更新

	// ゲット関数群
	// クオータニオン表記をXMFLOAT3値で出力
	XMFLOAT4 GetQuaternion(void) { 
		return quaternion; 
	}	 
	// クオータニオン表記をXMVECTOR値で出力
	XMVECTOR GetQuaternionV(void) {
		return XMLoadFloat4(&quaternion);
	}	 
	// クオータニオン表記をXMFLOAT3値で出力
	XMFLOAT4 GetQuaternionInv(void) {
		XMFLOAT4 q = GetQuaternion();
		q.x *= -1.0f;
		q.y *= -1.0f;
		q.z *= -1.0f;
		return q;
	}
	// クオータニオン表記をXMVECTOR値で出力
	XMVECTOR GetQuaternionInvV(void) {
		return XMLoadFloat4(&quaternion);
	}
	// 回転軸をXMFLOAT3値で出力
	XMFLOAT3 GetVectorF(void) {
		return axis;
	}		
	// 回転軸をXMVECTOR値で出力
	XMVECTOR GetVectorV(void) {
		return XMLoadFloat3(&axis); }
	// 角度を出力
	float GetAngle(void) {
		return angle;
	}
	// クオータニオンをオイラー値で出力
	XMFLOAT3 ConvertToEuler(void);
	XMFLOAT3* ConvertToEulerp(void);
	
	// セット関数群
	// クオータニオン表記の入力（XMVECTOR,XMFLOAT3対応）
	void SetQuaternion(XMFLOAT4 a_Quaternion) {
		quaternion = a_Quaternion;
		UpdateAA();
	}
	void SetQuaternion(XMVECTOR a_Quaternion) { 
		XMStoreFloat4(&quaternion, a_Quaternion);
		UpdateAA();
	}
	// 回転軸の入力（XMVECTOR,XMFLOAT3対応）
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
	// 角度の入力
	void SetAngle(float a_angle) {
		/*if (a_angle > 1.0f) { a_angle -= 1.0f; }
		else if (a_angle < -1.0f) { a_angle += 1.0f; }*/
		angle = a_angle;
		UpdateQuaternion();
	}

	
};

XMFLOAT3 RotateVectorFloatByQuaternion(XMFLOAT3 vector, Quaternion quaternion);
XMVECTOR RotateVectorByQuaternion(XMVECTOR vector, Quaternion quaternion);
float Dot(XMVECTOR* vectorDA, XMVECTOR* vectorCB);			// 内積計算
bool IsSameDir(XMVECTOR* vectorDA, XMVECTOR* vectorCB);		// ２つのベクトルが同じ方向を向いてるか？
XMVECTOR Cross(XMVECTOR* vectorDA, XMVECTOR* vectorCB);		// 外積計算（*順番に注意*）
XMFLOAT4 EulerToQuaternion(XMFLOAT3 euler);					// オイラー値をクオータニオン表記に変換
XMFLOAT3 HamiltonVQ(XMFLOAT3 vector, Quaternion quaternion);
XMFLOAT4 HamiltonQQ(XMFLOAT4 q, XMFLOAT4 q2);
float GetDistance(XMFLOAT3 vector);
float faceAngle(XMFLOAT3 vector);