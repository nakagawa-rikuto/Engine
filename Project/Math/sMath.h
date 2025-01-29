#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include "MatrixMath.h"

/// <summary>
/// π
/// </summary>
/// <returns></returns>
float Pi();

///=====================================================/// 
/// ここから各々で使用する関数を書き込む
///=====================================================///

#pragma region 変換
/// <summary>
/// 座標変換
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns></returns>
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);
#pragma endregion

/// <summary>
/// クロス積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 円と矩形の衝突判定
/// </summary>
/// <param name="circleCenter"></param>
/// <param name="circleRadius"></param>
/// <param name="rectCenter"></param>
/// <param name="reectHalfSize"></param>
/// <returns></returns>
bool IsCircleIntersectingRectangle(
	const Vector2& circleCenter, float circleRadius,
	const Vector2& rectCenter, const Vector2& rectHalfSize);

/// <summary>
/// 球体とAABBの衝突判定
/// </summary>
/// <param name="sphereCenter"></param>
/// <param name="sphereRadius"></param>
/// <param name="aabbMin"></param>
/// <param name="aabbMax"></param>
/// <returns></returns>
bool IsSphereIntersectingAABB(
	const Vector3& sphereCenter, float sphereRadius,
	const Vector3& aabbMin, const Vector3& aabbMax);