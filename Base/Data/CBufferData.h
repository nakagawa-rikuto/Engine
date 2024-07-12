#pragma once
#include "VectorData.h"
#include "MatrixData.h"

#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

/// <summary>
/// 三角形
/// </summary>
struct VertexDataTriangle {

	Vector4 position;
};

/// <summary>
/// スフィア
/// </summary>
struct VertexDataSphere {

	Vector4 position;
};

/// <summary>
/// マテリアル
/// </summary>
struct MaterialData {

	Vector4 color;
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
/// 
/// </summary>
struct TransformationMatrix {

	Matrix4x4 WVP;
};