#include "main.h"
#include "Quaternion.h"

XMFLOAT3 Quaternion::ConvertToEuler() {
	XMFLOAT3 result;

	//ロール(x)
	float sinr = 2 * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
	float cosr = 1 - 2 * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
	result.x = atan2f(sinr, cosr);

	//ピッチ(y)
	float sinp = sqrtf(1 + 2 * (quaternion.w * quaternion.y - quaternion.x * quaternion.z));
	float cosp = sqrtf(1 - 2 * (quaternion.w * quaternion.y - quaternion.x * quaternion.z));
	result.y = 2 * atan2f(sinp, cosp) - XM_PI / 2;

	//ヨー(z)
	float siny = 2 * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
	float cosy = 1 - 2 * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	result.z = atan2f(siny, cosy);

	return result;
}

XMFLOAT3* Quaternion::ConvertToEulerp() {
	XMFLOAT3 result;

	//ロール(x)
	float sinr = 2 * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
	float cosr = 1 - 2 * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
	result.x = atan2f(sinr, cosr);

	//ピッチ(y)
	float sinp = sqrtf(1 + 2 * (quaternion.w * quaternion.y - quaternion.x * quaternion.z));
	float cosp = sqrtf(1 - 2 * (quaternion.w * quaternion.y - quaternion.x * quaternion.z));
	result.y = 2 * atan2f(sinp, cosp) - XM_PI / 2;

	//ヨー(z)
	float siny = 2 * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
	float cosy = 1 - 2 * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	result.z = atan2f(siny, cosy);

	return &result;
}

void Quaternion::UpdateQuaternion() {
	quaternion.x = axis.x * sinf(angle * 0.5f);	//q2
	quaternion.y = axis.y * sinf(angle * 0.5f);	//q3
	quaternion.z = axis.z * sinf(angle * 0.5f);	//q4
	quaternion.w = cosf(angle * 0.5f);			//q1
}

void Quaternion::UpdateAA() {
	// クオータニオンのｗ値の二乗（計算省略の為）
	float qwsq = quaternion.w * quaternion.w;
	angle = 2 * acosf(quaternion.w);
	axis.x = quaternion.x / sqrt(1 - qwsq);
	axis.y = quaternion.y / sqrt(1 - qwsq);
	axis.z = quaternion.z / sqrt(1 - qwsq);
}

XMFLOAT4 EulerToQuaternion(XMFLOAT3 euler) {
	float cr = cosf(euler.x * 0.5);
	float sr = sinf(euler.x * 0.5);
	float cp = cosf(euler.y * 0.5);
	float sp = sinf(euler.y * 0.5);
	float cy = cosf(euler.z * 0.5);
	float sy = sinf(euler.z * 0.5);

	XMFLOAT4 q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

XMFLOAT3 RotateVectorFloatByQuaternion(XMFLOAT3 vector, Quaternion quaternion) {
	XMFLOAT4 vq = XMFLOAT4{ vector.x,vector.y,vector.z,0.0f };
	vq = HamiltonQQ(HamiltonQQ(quaternion.GetQuaternion(), vq), quaternion.GetQuaternionInv());
	XMFLOAT3 result = { vq.x,vq.y,vq.z };
	return result;
}
XMVECTOR RotateVectorByQuaternion(XMVECTOR vector, Quaternion quaternion) {
	XMFLOAT3 input;
	XMStoreFloat3(&input, vector);
	input = RotateVectorFloatByQuaternion(input, quaternion);
	return XMLoadFloat3(&input);
}

XMFLOAT3 HamiltonVQ(XMFLOAT3 vector, Quaternion quaternion) {
	XMFLOAT3 result = XMFLOAT3{ 0.0f,0.0f,0.0f };
	XMFLOAT4 q = quaternion.GetQuaternion();

	result.x = q.w * vector.x + q.x * 0.0f		+ q.y * vector.z - q.z * vector.y;
	result.y = q.w * vector.y - q.x * vector.z	+ q.y * 0.0f	 + q.z * vector.x;
	result.z = q.w * vector.z + q.x * vector.y	- q.y * vector.x + q.z * 0.0f;
	return result;
}
//1のクオータニオンの後２のクオータニオン
XMFLOAT4 HamiltonQQ( XMFLOAT4 q, XMFLOAT4 q2) {
	XMFLOAT4 result = XMFLOAT4{ 0.0f,0.0f,0.0f,0.0f };
	result.w = q2.w * q.w - q2.x * q.x - q2.y * q.y - q2.z * q.z;
	result.x = q2.w * q.x + q2.x * q.w + q2.y * q.z - q2.z * q.y;
	result.y = q2.w * q.y - q2.x * q.z + q2.y * q.w + q2.z * q.x;
	result.z = q2.w * q.z + q2.x * q.y - q2.y * q.x + q2.z * q.w;
	return result;
}
//=============================================================================
// 内積(dot)
//=============================================================================
float Dot(XMVECTOR* vectorDA, XMVECTOR* vectorCB)
{
	XMVECTOR temp = XMVector3Dot(*vectorDA, *vectorCB);
	float ans = 0.0f;
	XMStoreFloat(&ans, temp);
	return(ans);
}
bool IsSameDir(XMVECTOR* vectorDA, XMVECTOR* vectorCB) {
	return Dot(vectorDA, vectorCB) > 0;
}
//=============================================================================
// 外積(cross)
//=============================================================================
XMVECTOR Cross(XMVECTOR* vectorDA, XMVECTOR* vectorCB) {
	XMVECTOR result;
	result = XMVector3Cross(*vectorDA, *vectorCB);
	return result;
}

float GetDistance(XMFLOAT3 vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

float faceAngle(XMFLOAT3 vector) {
	if (vector.x == 0 || vector.z == 0) return 0.0f;
	float angle =  atanf(vector.z / vector.x);
	// 絶対値を求める
	if (angle > 0.0f) {
		angle *= -1.0f;
	}

	if (vector.x > 0.0f) {
		if (vector.z < 0.0f) {
			// Q2の場合
			angle *=  -1.0f;
		}
		angle += XM_PI * 0.5f;
	}
	else{
		if (vector.z > 0.0f) {
			//Q4の場合
			angle *= -1.0f;
		}
		angle -= XM_PI * 0.5f;
	}
	
	return angle;
}