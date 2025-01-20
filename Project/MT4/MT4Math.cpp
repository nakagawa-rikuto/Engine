#include "MT4Math.h"
#include <cstdio>

#include "Math/sMath.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
MT4::MT4() = default;
MT4::~MT4() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void MT4::Initialze() {
	rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.71f, 0.0f }, 0.3f);
	rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.0f, 0.71f }, Pi());

	interpolate0 = Slerp(rotation0, rotation1, 0.0f);
	interpolate1 = Slerp(rotation0, rotation1, 0.3f);
	interpolate2 = Slerp(rotation0, rotation1, 0.5f);
	interpolate3 = Slerp(rotation0, rotation1, 0.7f);
	interpolate4 = Slerp(rotation0, rotation1, 1.0f);
}

///-------------------------------------------/// 
/// 描画用ImGui
///-------------------------------------------///
void MT4::DraImgui() {
#ifdef USE_IMGUI

	ImGui::Begin("MT4");
	ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f  :  interpolate0, Slerp(q0,  q1,  0.0f)", interpolate0.x, interpolate0.y, interpolate0.z, interpolate0.w);
	ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f  :  interpolate1, Slerp(q0,  q1,  0.3f)", interpolate1.x, interpolate1.y, interpolate1.z, interpolate1.w);
	ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f  :  interpolate2, Slerp(q0,  q1,  0.5f)", interpolate2.x, interpolate2.y, interpolate2.z, interpolate2.w);
	ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f  :  interpolate3, Slerp(q0,  q1,  0.7f)", interpolate3.x, interpolate3.y, interpolate3.z, interpolate3.w);
	ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f  :  interpolate4, Slerp(q0,  q1,  1.0f)", interpolate4.x, interpolate4.y, interpolate4.z, interpolate4.w);
	ImGui::End();

#endif // USE_IMGUI

}

#pragma region Matrix
///-------------------------------------------/// 
/// 任意軸回転行列の関数
///-------------------------------------------///
Matrix4x4 MT4::MakeRotateAxisAngle(const Vector3& axis, float angle) {
	// 回転軸を正規化
	Vector3 normalizedAxis = Normalize(axis);

	float x = normalizedAxis.x;
	float y = normalizedAxis.y;
	float z = normalizedAxis.z;

	float cosTheta = std::cos(angle);
	float sinTheta = std::sin(angle);
	float oneMinusCosTheta = 1.0f - cosTheta;

	Matrix4x4 rotationMatrix;

	// ロドリゲスの回転公式に基づく計算
	rotationMatrix.m[0][0] = cosTheta + x * x * oneMinusCosTheta;
	rotationMatrix.m[0][1] = x * y * oneMinusCosTheta - z * sinTheta;
	rotationMatrix.m[0][2] = x * z * oneMinusCosTheta + y * sinTheta;
	rotationMatrix.m[0][3] = 0.0f;

	rotationMatrix.m[1][0] = y * x * oneMinusCosTheta + z * sinTheta;
	rotationMatrix.m[1][1] = cosTheta + y * y * oneMinusCosTheta;
	rotationMatrix.m[1][2] = y * z * oneMinusCosTheta - x * sinTheta;
	rotationMatrix.m[1][3] = 0.0f;

	rotationMatrix.m[2][0] = z * x * oneMinusCosTheta - y * sinTheta;
	rotationMatrix.m[2][1] = z * y * oneMinusCosTheta + x * sinTheta;
	rotationMatrix.m[2][2] = cosTheta + z * z * oneMinusCosTheta;
	rotationMatrix.m[2][3] = 0.0f;

	rotationMatrix.m[3][0] = 0.0f;
	rotationMatrix.m[3][1] = 0.0f;
	rotationMatrix.m[3][2] = 0.0f;
	rotationMatrix.m[3][3] = 1.0f;

	return rotationMatrix;
}

///-------------------------------------------/// 
/// ある方向からある方向への回転
///-------------------------------------------///
Matrix4x4 MT4::DirectionToDirection(const Vector3& from, const Vector3& to) {
	// 入力ベクトルが正規化されていない場合は正規化する
	Vector3 fromNormalized = Normalize(from);
	Vector3 toNormalized = Normalize(to);

	// ベクトルがほぼ同じ場合は単位行列を返す
	float dotProduct = fromNormalized.x * toNormalized.x + fromNormalized.y * toNormalized.y + fromNormalized.z * toNormalized.z;
	if (fabs(dotProduct - 1.0f) < 1e-6f) {
		Matrix4x4 identity;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				identity.m[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}
		return identity;
	}

	// ベクトルが反対方向の場合は適当な回転軸を使用して180度回転
	if (fabs(dotProduct + 1.0f) < 1e-6f) {
		Vector3 axis = { 1.0f, 0.0f, 0.0f };
		if (fabs(fromNormalized.x) < fabs(fromNormalized.y) && fabs(fromNormalized.x) < fabs(fromNormalized.z)) {
			axis = { 0.0f, -fromNormalized.z, fromNormalized.y }; // X 軸に垂直
		} else if (fabs(fromNormalized.y) < fabs(fromNormalized.z)) {
			axis = { -fromNormalized.z, 0.0f, fromNormalized.x }; // Y 軸に垂直
		} else {
			axis = { -fromNormalized.y, fromNormalized.x, 0.0f }; // Z 軸に垂直
		}
		float length = Length(axis);
		axis = { axis.x / length, axis.y / length, axis.z / length };

		Matrix4x4 result = {};
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = (i == j) ? -1.0f : 0.0f;
			}
		}
		axis = Normalize(axis);
		float c = -1.0f;
		float s = 0.0f;
		float t = 1.0f - c;

		Matrix4x4 rotation = {};
		rotation.m[0][0] = t * axis.x * axis.x + c;
		rotation.m[0][1] = t * axis.x * axis.y - s * axis.z;
		rotation.m[0][2] = t * axis.x * axis.z + s * axis.y;
		rotation.m[1][0] = t * axis.x * axis.y + s * axis.z;
		rotation.m[1][1] = t * axis.y * axis.y + c;
		rotation.m[1][2] = t * axis.y * axis.z - s * axis.x;
		rotation.m[2][0] = t * axis.x * axis.z - s * axis.y;
		rotation.m[2][1] = t * axis.y * axis.z + s * axis.x;
		rotation.m[2][2] = t * axis.z * axis.z + c;
		rotation.m[3][3] = 1.0f;

		return rotation;
	}

	/// ===一般的なケース: 回転軸と角度を計算=== ///
	Vector3 rotationAxis = {
		fromNormalized.y * toNormalized.z - fromNormalized.z * toNormalized.y,
		fromNormalized.z * toNormalized.x - fromNormalized.x * toNormalized.z,
		fromNormalized.x * toNormalized.y - fromNormalized.y * toNormalized.x
	};
	float axisLength = Length(rotationAxis);
	rotationAxis = { rotationAxis.x / axisLength, rotationAxis.y / axisLength, rotationAxis.z / axisLength };

	float angle = acosf(std::fmax(-1.0f, std::fmin(1.0f, dotProduct)));

	// 回転行列を生成 (ロドリゲスの回転公式を使用)
	Matrix4x4 rotation = {};
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0f - c;

	rotation.m[0][0] = t * rotationAxis.x * rotationAxis.x + c;
	rotation.m[0][1] = t * rotationAxis.x * rotationAxis.y - s * rotationAxis.z;
	rotation.m[0][2] = t * rotationAxis.x * rotationAxis.z + s * rotationAxis.y;
	rotation.m[1][0] = t * rotationAxis.x * rotationAxis.y + s * rotationAxis.z;
	rotation.m[1][1] = t * rotationAxis.y * rotationAxis.y + c;
	rotation.m[1][2] = t * rotationAxis.y * rotationAxis.z - s * rotationAxis.x;
	rotation.m[2][0] = t * rotationAxis.x * rotationAxis.z - s * rotationAxis.y;
	rotation.m[2][1] = t * rotationAxis.y * rotationAxis.z + s * rotationAxis.x;
	rotation.m[2][2] = t * rotationAxis.z * rotationAxis.z + c;
	rotation.m[3][3] = 1.0f;

	return rotation;
}
#pragma endregion

#pragma region Quaternion

///-------------------------------------------/// 
/// 任意軸回転を表すQuaternionの生成
///-------------------------------------------///
Quaternion MT4::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	Vector3 normalizedAxis = Normalize(axis);
	float sinHalfAngle = sinf(angle * 0.5f);
	float cosHalfAngle = cosf(angle * 0.5f);

	Quaternion quaternion = {
		normalizedAxis.x * sinHalfAngle,
		normalizedAxis.y * sinHalfAngle,
		normalizedAxis.z * sinHalfAngle,
		cosHalfAngle
	};

	// 正規化
	float length = sqrtf(quaternion.x * quaternion.x + quaternion.y * quaternion.y +
		quaternion.z * quaternion.z + quaternion.w * quaternion.w);
	if (length > 0.0f) {
		quaternion.x /= length;
		quaternion.y /= length;
		quaternion.z /= length;
		quaternion.w /= length;
	}

	return quaternion;
}

///-------------------------------------------/// 
/// ベクトルをQuaternionで回転させた結果のベクトルを求める
///-------------------------------------------///
Vector3 MT4::RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	Quaternion qVector = { vector.x, vector.y, vector.z, 0.0f };

	// Quaternion conjugate
	Quaternion conjugate = {
		-quaternion.x,
		-quaternion.y,
		-quaternion.z,
		quaternion.w
	};

	// Perform rotation: q * v * q^*
	Quaternion rotated = Multiply(Multiply(quaternion, qVector), conjugate);

	return Vector3{ rotated.x, rotated.y, rotated.z };
}

///-------------------------------------------/// 
/// Quaternionから回転行列を求める
///-------------------------------------------///
Matrix4x4 MT4::MakeRotateMatrix(const Quaternion& quaternion) {
	float xx = quaternion.x * quaternion.x;
	float yy = quaternion.y * quaternion.y;
	float zz = quaternion.z * quaternion.z;
	float xy = quaternion.x * quaternion.y;
	float xz = quaternion.x * quaternion.z;
	float yz = quaternion.y * quaternion.z;
	float wx = quaternion.w * quaternion.x;
	float wy = quaternion.w * quaternion.y;
	float wz = quaternion.w * quaternion.z;

	return Matrix4x4{
		// 列1
		1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz),       2.0f * (xz - wy),       0.0f,
		// 列2
		2.0f * (xy - wz),       1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx),       0.0f,
		// 列3
		2.0f * (xz + wy),       2.0f * (yz - wx),       1.0f - 2.0f * (xx + yy), 0.0f,
		// 列4
		0.0f,                   0.0f,                   0.0f,                   1.0f
	};
}

///-------------------------------------------/// 
/// 球面線形補間
///-------------------------------------------///
Quaternion MT4::Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
	// tを[0, 1]の範囲にクランプ
	t = fmaxf(0.0f, fminf(1.0f, t));

	// Quaternion同士の内積（角度のコサイン）を計算
	float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

	// 内積が負の場合、短い経路を取るために一方のQuaternionを反転
	Quaternion q0Modified = q0;
	Quaternion q1Modified = q1;
	if (dot < 0.0f) {
		q0Modified.x = -q0.x;
		q0Modified.y = -q0.y;
		q0Modified.z = -q0.z;
		q0Modified.w = -q0.w;
		dot = -dot;
	}

	// なす角を計算
	float theta = acosf(dot);
	float sinTheta = sinf(theta);

	// 補間係数を計算
	float scale0 = sinf((1.0f - t) * theta) / sinTheta;
	float scale1 = sinf(t * theta) / sinTheta;

	// 補間後のQuaternionを計算
	Quaternion result = Quaternion{
		scale0 * q0Modified.x + scale1 * q1Modified.x,
		scale0 * q0Modified.y + scale1 * q1Modified.y,
		scale0 * q0Modified.z + scale1 * q1Modified.z,
		scale0 * q0Modified.w + scale1 * q1Modified.w
	};

	// 結果を正規化
	result = Normalize(result);
	return result;
}

#pragma endregion







