#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/CData.h"
#include "Engine/Core/ComPtr.h"
// Buffer
#include "IndexBuffer3D.h"
#include "Material3D.h"
#include "Transform3D.h"
#include "Light.h"
#include "Camera3D.h"
// c++
#include <memory>

///=====================================================/// 
/// モデル共通部
///=====================================================///
class ModelCommon {
public:
	ModelCommon();
	~ModelCommon();

	// 初期化
	void Initialize(ID3D12Device* device, LightType type); // オブジェクトを読み込まない場合の初期化
	// 描画
	void Bind(ID3D12GraphicsCommandList* commandList);

public:/// ===Setter=== ///
	// Material
	void SetMatiarlData(const Vector4& color, const float& shininess, const Matrix4x4& uvTransform);
	// ワールドトランスフォーム
	void SetTransformData(const Matrix4x4& WVP, const Matrix4x4& World, const Matrix4x4& WorldInverseTranspose);
	// Light
	void SetDirectionLight(const Vector4& color, const Vector3& direction, const float& intensity);
	// CameraForGPU
	void SetCameraForGPU(const Vector3& translate);

private:/// ===Variables(変数)=== ///

	// バッファリソース
	std::unique_ptr<Material3D> material_;
	std::unique_ptr<Transform3D> wvp_;
	std::unique_ptr<Light> light_;
	std::unique_ptr<Camera3D> camera3D_;

	// バッファリソース内のデータを指すポインタ
	MaterialData3D* materialData_ = nullptr;
	TransformationMatrix3D* wvpMatrixData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	CameraForGPU* cameraForGPUData_ = nullptr;
};

