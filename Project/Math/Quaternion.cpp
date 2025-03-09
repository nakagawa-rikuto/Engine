
#include "Quaternion.h"
#include "Math/sMath.h"

///-------------------------------------------/// 
/// Quaternionのnormを返す
///-------------------------------------------///
float QuatMath::Norm(const Quaternion& quaternion) {
	return sqrtf(quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w);
}

///-------------------------------------------/// 
/// Quaternionの積
///-------------------------------------------///
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	return {
		 lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
		 lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
		 lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
		 lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	};
}

Quaternion Normalize(const Quaternion& quaternion) {
	float norm = QuatMath::Norm(quaternion);
	if (norm == 0.0f) {
		// Avoid division by zero
		return QuatMath::IdentityQuaternion();
	}
	return {
		quaternion.x / norm,
		quaternion.y / norm,
		quaternion.z / norm,
		quaternion.w / norm
	};
}

///-------------------------------------------/// 
/// 単位Quaternionを返す
///-------------------------------------------///
Quaternion QuatMath::IdentityQuaternion() {
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

///-------------------------------------------/// 
/// 共役Quaternionを返す
///-------------------------------------------///
Quaternion QuatMath::Conjugate(const Quaternion& quaternion) {
	return {
		-quaternion.x,
		-quaternion.y,
		-quaternion.z,
		quaternion.w
	};
}

///-------------------------------------------/// 
/// 逆Quaternionを返す
///-------------------------------------------///
Quaternion QuatMath::Inverse(const Quaternion& quaternion) {
	float normSquared = quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w;
	if (normSquared == 0.0f) {
		// Avoid division by zero
		return IdentityQuaternion();
	}
	Quaternion conjugate = Conjugate(quaternion);
	return {
		conjugate.x / normSquared,
		conjugate.y / normSquared,
		conjugate.z / normSquared,
		conjugate.w / normSquared
	};
}

///-------------------------------------------/// 
/// 任意軸回転Quaternionを返す
///-------------------------------------------///
Quaternion QuatMath::MakeRotateAxisAngle(const Vector3& axis, float angle) {
	Quaternion result;
	float halfAngle = angle / 2.0f;
	float sinHalfAngle = sinf(halfAngle);
	result.x = axis.x * sinHalfAngle;
	result.y = axis.y * sinHalfAngle;
	result.z = axis.z * sinHalfAngle;
	result.w = cosf(halfAngle);
	return result;
}

///-------------------------------------------/// 
/// Quaternionの回転結果をベクトルで返す
///-------------------------------------------///
Vector3 QuatMath::RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	Vector3 result;
	Quaternion q = quaternion;
	Quaternion v = { vector.x,vector.y,vector.z,0.0f };
	Quaternion qConjugate = Conjugate(q);
	Quaternion v1 = Multiply(q, v);
	Quaternion v2 = Multiply(v1, qConjugate);
	result.x = v2.x;
	result.y = v2.y;
	result.z = v2.z;
	return result;
}

///-------------------------------------------/// 
/// Quaternionから角度を取得する関数
///-------------------------------------------///
// X
float QuatMath::GetXAngle(const Quaternion& quaternion) {
	// 単位ベクトル (0,0,1) をクォータニオンで回転させる
	Vector3 forward = RotateVector(Vector3(0.0f, 0.0f, 1.0f), quaternion);
	// forward.y がピッチ角度（上向きか下向きか）を示す
	return std::asin(forward.y); // ラジアン角
}
// Y
float QuatMath::GetYAngle(const Quaternion& quaternion) {
	// 単位ベクトル (0,0,1) をクォータニオンで回転させる
	Vector3 forward = RotateVector(Vector3(0.0f, 0.0f, 1.0f), quaternion);
	// atan2(forward.x, forward.z) で Yaw（水平回転） を取得。
	return std::atan2(forward.x, forward.z); // Yaw 角度（ラジアン）
}



///-------------------------------------------/// 
/// オペレータ
///-------------------------------------------///
 /// === 加算 === ///
Quaternion Quaternion::operator+(const float& v) { return Quaternion{ x + v, y + v, z + v, w + v }; }
Quaternion& Quaternion::operator+=(const float& v) {
    x += v; y += v; z += v; w += v;
    return *this;
}
Quaternion Quaternion::operator+(const Quaternion& q) { return Quaternion{ x + q.x, y + q.y, z + q.z, w + q.w }; }
Quaternion& Quaternion::operator+=(const Quaternion& q) {
    x += q.x; y += q.y; z += q.z; w += q.w;
    return *this;
}
Quaternion Quaternion::operator+(const float& v) const { return Quaternion{ x + v, y + v, z + v, w + v }; }
Quaternion Quaternion::operator+(const Quaternion& q) const { return Quaternion{ x + q.x, y + q.y, z + q.z, w + q.w }; }

/// === 減算 === ///
Quaternion Quaternion::operator-(const float& v) { return Quaternion{ x - v, y - v, z - v, w - v }; }
Quaternion& Quaternion::operator-=(const float& v) {
    x -= v; y -= v; z -= v; w -= v;
    return *this;
}
Quaternion Quaternion::operator-(const Quaternion& q) { return Quaternion{ x - q.x, y - q.y, z - q.z, w - q.w }; }
Quaternion& Quaternion::operator-=(const Quaternion& q) {
    x -= q.x; y -= q.y; z -= q.z; w -= q.w;
    return *this;
}
Quaternion Quaternion::operator-(const float& v) const { return Quaternion{ x - v, y - v, z - v, w - v }; }
Quaternion Quaternion::operator-(const Quaternion& q) const { return Quaternion{ x - q.x, y - q.y, z - q.z, w - q.w }; }

/// === 乗算 === ///
// スカラー倍
Quaternion Quaternion::operator*(const float& v) { return Quaternion{ x * v, y * v, z * v, w * v }; }
Quaternion& Quaternion::operator*=(const float& v) {
    x *= v; y *= v; z *= v; w *= v;
    return *this;
}
Quaternion Quaternion::operator*(const float& v) const { return Quaternion{ x * v, y * v, z * v, w * v }; }

// クォータニオン乗算
Quaternion Quaternion::operator*(const Quaternion& q) {
    return Quaternion{
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w,
        w * q.w - x * q.x - y * q.y - z * q.z
    };
}
Quaternion& Quaternion::operator*=(const Quaternion& q) {
    *this = *this * q;
    return *this;
}
Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion{
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w,
        w * q.w - x * q.x - y * q.y - z * q.z
    };
}

/// === 除算 === ///
Quaternion Quaternion::operator/(const float& v) { return Quaternion{ x / v, y / v, z / v, w / v }; }
Quaternion& Quaternion::operator/=(const float& v) {
    x /= v; y /= v; z /= v; w /= v;
    return *this;
}
Quaternion Quaternion::operator/(const float& v) const { return Quaternion{ x / v, y / v, z / v, w / v }; }

// クォータニオンの除算（逆クォータニオンをかける）
Quaternion Quaternion::operator/(const Quaternion& q) {
    return (*this) * QuatMath::Inverse(q);
}
Quaternion& Quaternion::operator/=(const Quaternion& q) {
    *this = *this / q;
    return *this;
}
Quaternion Quaternion::operator/(const Quaternion& q) const {
    return (*this) * QuatMath::Inverse(q);
}
