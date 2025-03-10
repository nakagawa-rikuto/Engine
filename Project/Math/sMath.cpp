#include "sMath.h"
#include <cassert>
#include <numbers>

///=====================================================///
/// π（πを使用する際に用いる関数）
///=====================================================///
float Pi() { return std::numbers::pi_v<float>; }

///=====================================================///
/// クロス積の計算
///=====================================================///
Vector3 Cross(const Vector3& v1, const Vector3& v2) {

    Vector3 result{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };

    return result;
}