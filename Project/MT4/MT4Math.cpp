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
	rotate = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f, 0.4f, -0.2f }), 0.45f);
	pointY = { 2.1f, -0.9f, 1.3f };
	rotateMatrix = MakeRotateMatrix(rotate);
	rotateByQuaternion = RotateVector(pointY, rotate);
	rotateByMatrix = TransformCoordinates(pointY, rotateMatrix);
}

///-------------------------------------------/// 
/// 描画用ImGui
///-------------------------------------------///
void MT4::DraImgui() {
#ifdef USE_IMGUI

	ImGui::Begin("MT4");
	ImGui::Text("%5.3f  %5.3f  %5.3f  %5.3f  :  rotation", rotate.x, rotate.y, rotate.z, rotate.w);
	ImGui::Text("rotateMatrix");
	ImGui::Text("%5.3f  %5.3f  %5.3f  %5.3f", rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3]);
	ImGui::Text("%5.3f  %5.3f  %5.3f  %5.3f", rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3]);
	ImGui::Text("%5.3f  %5.3f  %5.3f  %5.3f", rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3]);
	ImGui::Text("%5.3f  %5.3f  %5.3f  %5.3f", rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);
	ImGui::Text("%5.3f  %5.3f  %5.3f         :  rotateByQuaternion", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
	ImGui::Text("%5.3f  %5.3f  %5.3f         :  rotateByMatrix", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);
	ImGui::End();

#endif // USE_IMGUI

}

#pragma region Quaternion

///-------------------------------------------/// 
/// 任意軸回転を表すQuaternionの生成
///-------------------------------------------///
Quaternion MT4::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	Vector3 normalizedAxis = Normalize(axis);
	float sinHalfAngle = sinf(angle * 0.5f);
	float cosHalfAngle = cosf(angle * 0.5f);

	return Quaternion{
		normalizedAxis.x * sinHalfAngle,
		normalizedAxis.y * sinHalfAngle,
		normalizedAxis.z * sinHalfAngle,
		cosHalfAngle
	};
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
		1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz),       2.0f * (xz + wy),       0.0f,
		2.0f * (xy + wz),       1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx),       0.0f,
		2.0f * (xz - wy),       2.0f * (yz + wx),       1.0f - 2.0f * (xx + yy), 0.0f,
		0.0f,                   0.0f,                   0.0f,                   1.0f
	};
}

#pragma endregion
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






