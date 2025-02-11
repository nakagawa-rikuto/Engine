#include "EasingMath.h"
#define _USE_MATH_DEFINES
#include <cmath>


///-------------------------------------------/// 
/// Lerp関数
///-------------------------------------------///
// Vector3
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;

	result.x = (1.0f - t) * v1.x + t * v2.x;
	result.y = (1.0f - t) * v1.y + t * v2.y;
	result.z = (1.0f - t) * v1.z + t * v2.z;

	return result;
}
// Quaternion
Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t) {
	Quaternion result;

	result.x = (1.0f - t) * q1.x + t * q2.x;
	result.y = (1.0f - t) * q1.y + t * q2.y;
	result.z = (1.0f - t) * q1.z + t * q2.z;
	result.w = (1.0f - t) * q1.w + t * q2.w;

	return result;
}

///-------------------------------------------/// 
/// SLerp関数
///-------------------------------------------///
// Vector3
Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t) {
	// なす角の計算
	float angle = std::cosf(Dot(v1, v2));

	// 線形補間を計算する
	float scaleV1 = std::sinf((1.0f - t) * angle) / std::sinf(angle);

	float scaleV2 = std::sinf(t * angle) / std::sinf(angle);

	Vector3 result;

	result.x = scaleV1 * v1.x + scaleV2 * v2.x;
	result.y = scaleV1 * v1.y + scaleV2 * v2.y;
	result.z = scaleV1 * v1.z + scaleV2 * v2.z;

	return result;
}
// Quaternion
Quaternion SLerp(const Quaternion& q1, const Quaternion& q2, float t) {
    // q1 と q2 の内積を計算
    float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

    // 内積が負の場合、補間経路を反転させる
    Quaternion q2Corrected = q2;
    if (dot < 0.0f) {
        q2Corrected.x = -q2.x;
        q2Corrected.y = -q2.y;
        q2Corrected.z = -q2.z;
        q2Corrected.w = -q2.w;
        dot = -dot;
    }

    // 内積が 1 に近い場合（角度が小さい）、Lerp で近似
    const float THRESHOLD = 0.9995f;
    if (dot > THRESHOLD) {
        Quaternion result = {
            (1.0f - t) * q1.x + t * q2Corrected.x,
            (1.0f - t) * q1.y + t * q2Corrected.y,
            (1.0f - t) * q1.z + t * q2Corrected.z,
            (1.0f - t) * q1.w + t * q2Corrected.w
        };
        return Normalize(result);
    }

    // 角度を計算
    float theta = std::acos(dot);
    float sinTheta = std::sin(theta);

    // 補間係数を計算
    float w1 = std::sin((1.0f - t) * theta) / sinTheta;
    float w2 = std::sin(t * theta) / sinTheta;

    // Slerp による補間
    Quaternion result = {
        w1 * q1.x + w2 * q2Corrected.x,
        w1 * q1.y + w2 * q2Corrected.y,
        w1 * q1.z + w2 * q2Corrected.z,
        w1 * q1.w + w2 * q2Corrected.w
    };

    return result;
}
