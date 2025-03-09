#include "EasingMath.h"
#define _USE_MATH_DEFINES
#include <cmath>




///-------------------------------------------/// 
/// Lerp関数
///-------------------------------------------///
// float
float Lerp(float start, float end, float t) {
    return start * (1.0f - t) + end * t;
}
// Vector3
Vector3 Lerp(const Vector3& start, const Vector3& end, float t) {
	Vector3 result;

	result.x = (1.0f - t) * start.x + t * end.x;
	result.y = (1.0f - t) * start.y + t * end.y;
	result.z = (1.0f - t) * start.z + t * end.z;

	return result;
}
// Quaternion
Quaternion Lerp(const Quaternion& start, const Quaternion& end, float t) {
	Quaternion result;

	result.x = (1.0f - t) * start.x + t * end.x;
	result.y = (1.0f - t) * start.y + t * end.y;
	result.z = (1.0f - t) * start.z + t * end.z;
	result.w = (1.0f - t) * start.w + t * end.w;

	return result;
}

///-------------------------------------------/// 
/// SLerp関数
///-------------------------------------------///
// Vector3
Vector3 SLerp(const Vector3& start, const Vector3& end, float t) {
	// なす角の計算
	float angle = std::cosf(Dot(start, end));

	// 線形補間を計算する
	float scalestart = std::sinf((1.0f - t) * angle) / std::sinf(angle);

	float scaleend = std::sinf(t * angle) / std::sinf(angle);

	Vector3 result;

	result.x = scalestart * start.x + scaleend * end.x;
	result.y = scalestart * start.y + scaleend * end.y;
	result.z = scalestart * start.z + scaleend * end.z;

	return result;
}
// Quaternion
Quaternion SLerp(const Quaternion& start, const Quaternion& end, float t) {
    // start と end の内積を計算
    float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;

    // 内積が負の場合、補間経路を反転させる
    Quaternion endCorrected = end;
    if (dot < 0.0f) {
        endCorrected.x = -end.x;
        endCorrected.y = -end.y;
        endCorrected.z = -end.z;
        endCorrected.w = -end.w;
        dot = -dot;
    }

    // 内積が 1 に近い場合（角度が小さい）、Lerp で近似
    const float THRESHOLD = 0.9995f;
    if (dot > THRESHOLD) {
        return Normalize(Lerp(start, end, t));
    }

    // 角度を計算
    float theta = std::acos(dot);
    float sinTheta = std::sin(theta);

    // 補間係数を計算
    float w1 = std::sin((1.0f - t) * theta) / sinTheta;
    float w2 = std::sin(t * theta) / sinTheta;

    // Slerp による補間
    Quaternion result = {
        w1 * start.x + w2 * endCorrected.x,
        w1 * start.y + w2 * endCorrected.y,
        w1 * start.z + w2 * endCorrected.z,
        w1 * start.w + w2 * endCorrected.w
    };

    return result;
}
