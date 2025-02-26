#pragma once
#include "Vector3.h"
#include "Quaternion.h"

/// <summary>
/// ラープ関数（float）
/// </summary>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="t"></param>
/// <returns></returns>
float Lerp(float start, float end, float t);

/// <summary>
/// ラープ関数（Vector3）
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

/// <summary>
/// ラープ関数（Quaternion）
/// </summary>
/// <param name="q1"></param>
/// <param name="q2"></param>
/// <param name="t"></param>
/// <returns></returns>
Quaternion Lerp(const Quaternion& start, const Quaternion& end, float t);

/// <summary>
/// Sラープ関数（Vector3）
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 SLerp(const Vector3& start, const Vector3& end, float t);

/// <summary>
/// Sラープ関数（Quaternion）
/// </summary>
/// <param name="q1"></param>
/// <param name="q2"></param>
/// <param name="t"></param>
/// <returns></returns>
Quaternion SLerp(const Quaternion& start, const Quaternion& end, float t);
