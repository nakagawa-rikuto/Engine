#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include "MathData.h"
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
