#pragma once
#include "Vector3.h"

/// <summary>
/// Quaternionの構造体
/// </summary>
struct Quaternion final {
	float x;
	float y;
	float z;
	float w;

	/// ===加法=== ///
	// Quaternion | const
	Quaternion operator+(const float& v);
	Quaternion& operator+=(const float& v);
	Quaternion operator+(const Quaternion& v);
	Quaternion& operator+=(const Quaternion& v);
	// const | const
	Quaternion operator+(const float& v)const;
	Quaternion operator+(const Quaternion& v)const;

	/// ===減法=== ///
	// Quaternion | const
	Quaternion operator-(const float& v);
	Quaternion& operator-=(const float& v);
	Quaternion operator-(const Quaternion& v);
	Quaternion& operator-=(const Quaternion& v);

	// const | const
	Quaternion operator-(const float& v)const;
	Quaternion operator-(const Quaternion& v)const;

	/// ===積=== ///
	// Quaternion | const
	Quaternion operator*(const float& v);
	Quaternion& operator*=(const float& v);
	Quaternion operator*(const Quaternion& v);
	Quaternion& operator*=(const Quaternion& v);
	// const | const
	Quaternion operator*(const float& v)const;
	Quaternion operator*(const Quaternion& v)const;

	/// ===除法=== ///
	// Quaternion | const
	Quaternion operator/(const float& v);
	Quaternion& operator/=(const float& v);
	Quaternion operator/(const Quaternion& v);
	Quaternion& operator/=(const Quaternion& v);
	// const | const
	Quaternion operator/(const float& v)const;
	Quaternion operator/(const Quaternion& v)const;
};

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
/// ===任意軸回転Quaternionを返す=== ///
Quaternion MakeRotateAxisAngle(const Vector3& axis, float angle);
/// ===Quaternionの回転結果をベクトルで返す=== ///
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);