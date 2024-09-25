#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "MatrixMath.h"

/// <summary>
/// π
/// </summary>
/// <returns></returns>
float Pi();

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
/// Vector3と行列の掛け算
/// </summary>
/// <param name="v"></param>
/// <param name="m"></param>
/// <returns></returns>
//Vector3 Multiply(const Vector3& v, const Matrix4x4& m);

/// <summary>
/// クロス積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);