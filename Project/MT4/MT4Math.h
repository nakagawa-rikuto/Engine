#pragma once
#include "Math/sMath.h"

#include <iostream>
#include <iomanip>


/// <summary>
/// 任意軸回転行列の関数
/// </summary>
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

/// <summary>
/// 行列をコンソールに描画する関数
/// </summary>
void PrintMatrix(const Vector2& position, const Matrix4x4& matrix, const std::string& name);
