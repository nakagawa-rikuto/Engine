#include "sMath.h"
#include <cassert>
#include <numbers>

///=====================================================///
/// π（πを使用する際に用いる関数）
///=====================================================///
float Pi() { return std::numbers::pi_v<float>; }

///=====================================================///
/// ベクトル変換
///=====================================================///
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix) {
    Vector3 result;
    result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0];
    result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1];
    result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2];
    return result;
}

///=====================================================///
/// クロス積の計算
///=====================================================///
Vector3 Cross(const Vector3& v1, const Vector3& v2) {

    Vector3 result{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };

    return result;
}