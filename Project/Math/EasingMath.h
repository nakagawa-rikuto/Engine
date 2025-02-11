#pragma once
#include "Vector3.h"
#include "Quaternion.h"

/// <summary>
/// ラープ関数（Vector3）
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// ラープ関数（Quaternion）
/// </summary>
/// <param name="q1"></param>
/// <param name="q2"></param>
/// <param name="t"></param>
/// <returns></returns>
Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);

/// <summary>
/// Sラープ関数（Vector3）
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// Sラープ関数（Quaternion）
/// </summary>
/// <param name="q1"></param>
/// <param name="q2"></param>
/// <param name="t"></param>
/// <returns></returns>
Quaternion SLerp(const Quaternion& q1, const Quaternion& q2, float t);
