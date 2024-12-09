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
#include "Math/MathData.h"

///-------------------------------------------/// 
/// BufferData
///-------------------------------------------///
// VertexData(2D)
struct VertexData2D {
	Vector4 position;
	Vector2 texcoord;
};
// VertexData(3D)
struct VertexData3D {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
// IndexData
struct IndexData {
	uint32_t data;
};
// Material(2D)
struct MaterialData2D {
	Vector4 color;
	Matrix4x4 uvTransform;
};
// Material(3D)
struct MaterialData3D {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
// TransformationMatrix(2D)
struct TransformationMatrix2D {
	Matrix4x4 WVP;
};
// TransformationMatrix(3D)
struct TransformationMatrix3D {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

///-------------------------------------------/// 
/// WorldTransform
///-------------------------------------------///
struct WorldTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

///-------------------------------------------///  
/// マテリアルデータ
///-------------------------------------------///
struct  MaterialData {
	std::string textureFilePath;
};

///-------------------------------------------/// 
/// モデルデータ
///-------------------------------------------///
struct ModelData {
	std::vector<VertexData3D> vertices;
	MaterialData material;
};

///-------------------------------------------/// 
/// 平行光源の拡張
///-------------------------------------------///
struct DirectionalLight {
	Vector4 color;     // ライトの色
	Vector3 direction; // ライトの向き
	float intensity;   // ライトの明るさ(輝度)
};

///-------------------------------------------/// 
/// パーティクル
///-------------------------------------------///
// Data
struct ParitcleData {
	WorldTransform transform;
	Vector3 velcotiy;
	Vector4 color;
	float lifeTime;
	float curretTime;
};
// 
struct ParitcleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};

///-------------------------------------------/// 
/// エミッタ
///-------------------------------------------///
struct Emitter {
	WorldTransform transform;
	uint32_t count;
	float frequency;
	float frequencyTime;
};

///-------------------------------------------/// 
/// Field
///-------------------------------------------///
struct AccelerationField {
	Vector3 acceleration;
	AABB area;
};