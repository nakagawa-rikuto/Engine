#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

///-------------------------------------------/// 
/// AABBの構造体
///-------------------------------------------///
struct AABB {
    Vector3 min; //!<最小点
    Vector3 max; //!<最大点
};