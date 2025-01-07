#include "sMath.h"
#include <cassert>
#include <numbers>
#include <algorithm>

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

///-------------------------------------------/// 
/// 円と矩形の衝突判定
///-------------------------------------------///
bool IsCircleIntersectingRectangle(const Vector2& circleCenter, float circleRadius, const Vector2& rectCenter, const Vector2& rectHalfSize) {
    // 矩形の最近接点を求める
    float closestX = std::clamp(circleCenter.x, rectCenter.x - rectHalfSize.x, rectCenter.x + rectHalfSize.x);
    float closestY = std::clamp(circleCenter.y, rectCenter.y - rectHalfSize.y, rectCenter.y + rectHalfSize.y);

    // 円の中心と最近接点の距離を計算
    float distanceX = circleCenter.x - closestX;
    float distanceY = circleCenter.y - closestY;

    // 距離の平方と半径の平方を比較
    return (distanceX * distanceX + distanceY * distanceY) <= (circleRadius * circleRadius);
}

///-------------------------------------------/// 
/// 球体とAABBの衝突判定
///-------------------------------------------///
bool IsSphereIntersectingAABB(const Vector3& sphereCenter, float sphereRadius, const Vector3& aabbMin, const Vector3& aabbMax) {
    // AABBの各辺で最近接点を求める
    float closestX = std::clamp(sphereCenter.x, aabbMin.x, aabbMax.x);
    float closestY = std::clamp(sphereCenter.y, aabbMin.y, aabbMax.y);
    float closestZ = std::clamp(sphereCenter.z, aabbMin.z, aabbMax.z);

    // 球体の中心と最近接点の距離を計算
    float distanceX = sphereCenter.x - closestX;
    float distanceY = sphereCenter.y - closestY;
    float distanceZ = sphereCenter.z - closestZ;

    // 距離の平方と半径の平方を比較
    return (distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ) <= (sphereRadius * sphereRadius);
}
