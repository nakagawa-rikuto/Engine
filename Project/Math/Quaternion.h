
#pragma once

/// <summary>
/// Quaternionの構造体
/// </summary>
struct Quaternion final {
	float x;
	float y;
	float z;
	float w;
};

Quaternion operator-(const Quaternion& q0);

/// ===Quaternionのnormを返す=== ///
float Norm(const Quaternion& quaternion);
/// ===Quaternionの積=== ///
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
/// ===正規化したQuaternionを返す=== ///
Quaternion Normalize(const Quaternion& quaternion);
/// ===単位Quaternionを返す=== ///
Quaternion IdentityQuaternion();
/// ===共役Quaternionを返す=== ///
Quaternion Conjugate(const Quaternion& quaternion);
/// ===逆Quaternionを返す=== ///
Quaternion Inverse(const Quaternion& quaternion);