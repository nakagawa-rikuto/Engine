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
/// <summary>
/// クロス積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);


