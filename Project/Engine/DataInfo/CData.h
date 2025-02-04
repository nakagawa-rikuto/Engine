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
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"

#pragma region Buffer関連
/// ===VertexData(2D)=== ///
struct VertexData2D {
	Vector4 position;
	Vector2 texcoord;
};
/// ===VertexData(3D)=== ///
struct VertexData3D {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
/// ===Material(2D)=== ///
struct MaterialData2D {
	Vector4 color;
	Matrix4x4 uvTransform;
};
/// ===Material(3D)=== ///
struct MaterialData3D {

	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
};
/// ===TransformationMatrix(2D)=== ///
struct TransformationMatrix2D {
	Matrix4x4 WVP;
};
/// ===TransformationMatrix(3D)=== ///
struct TransformationMatrix3D {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
};
#pragma endregion
#pragma region WorldTransform
/// ===WorldTransform=== ///
struct WorldTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
#pragma endregion
#pragma region モデル
/// ===マテリアルデータ=== ///
struct  MaterialData {
	std::string textureFilePath;
};
/// ===ノード=== ///
struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};
/// ===モデルデータ=== ///
struct ModelData {
	std::vector<VertexData3D> vertices;
	MaterialData material;
	Node rootNode;
};
#pragma endregion
#pragma region ライト
/// ===平行光源の拡張=== ///
struct DirectionalLight {
	Vector4 color;     // ライトの色
	Vector3 direction; // ライトの向き
	float intensity;   // ライトの明るさ(輝度)
};
/// ===カメラの位置=== ///
struct CameraForGPU {
	Vector3 worldPosition;
};
/// ===ポイントライト=== ///
struct PointLight {
	Vector4 color;
	Vector3 position;
	float intensity;
	float radius;
	float decay;
	float padding[2];
};
/// ===スポットライト=== ///
struct SpotLight {
	Vector4 color;
	Vector3 position;
	float intensity;
	Vector3 direction;
	float distance;
	float decay;
	float cosAngle;
	float padding[3];
};
#pragma endregion