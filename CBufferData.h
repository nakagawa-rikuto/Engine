#pragma once
#include "VectorData.h"

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

struct MaterialData {

	Vector4 color;
};


