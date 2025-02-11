
#include "Quaternion.h"
#include "Math/sMath.h"

///-------------------------------------------/// 
/// Quaternionのnormを返す
///-------------------------------------------///
float Norm(const Quaternion& quaternion) {
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
	float norm = Norm(quaternion);
	if (norm == 0.0f) {
		// Avoid division by zero
		return IdentityQuaternion();
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
Quaternion IdentityQuaternion() {
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

///-------------------------------------------/// 
/// 共役Quaternionを返す
///-------------------------------------------///
Quaternion Conjugate(const Quaternion& quaternion) {
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
Quaternion Inverse(const Quaternion& quaternion) {
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
/// オペレータ
///-------------------------------------------///
Quaternion operator-(const Quaternion& q0) {
	return { -q0.x, -q0.y, -q0.z, -q0.w };
}