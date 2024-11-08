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
/// VertexData(2D)
/// </summary>
struct VertexData2D {
	Vector4 position;
	Vector2 texcoord;
};

/// <summary>
/// VertexData(3D)
/// </summary>
struct VertexData3D {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

/// <summary>
/// MaterialData(2D)
/// </summary>
struct MaterialData2D {
	Vector4 color;
	Matrix4x4 uvTransform;
};

/// <summary>
/// MaterialData(3D)
/// </summary>
struct MaterialData3D {

	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

/// <summary>
/// TransformationMatrix(2D)
/// </summary>
struct TransformationMatrix2D {
	Matrix4x4 WVP;
};

/// <summary>
/// TransformationMatrix(3D)
/// </summary>
struct TransformationMatrix3D {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

/// <summary>
/// WorldTransform
/// </summary>
struct WorldTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
