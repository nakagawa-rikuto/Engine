#pragma once
/// ===include=== ///
// C++
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

// Math
#include "Base/Math/Vector2.h"
#include "Base/Math/Vector3.h"
#include "Base/Math/Vector4.h"
#include "Base/Math/Matrix3x3.h"
#include "Base/Math/Matrix4x4.h"

/// <summary>
/// VertexData
/// </summary>
struct VertexData {

	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

/// <summary>
/// マテリアル
/// </summary>
struct MaterialData {

	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

/// <summary>
/// 位置・姿勢情報
/// </summary>
struct TransformInfo {

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

/// <summary>
/// 座標変換行列データ
/// </summary>
struct TransformationMatrix {

	Matrix4x4 WVP;
	Matrix4x4 World;
};