#pragma once
/// ===include=== ///
// Buffer
#include "Engine/Graphics/3d/Base/Material3D.h"
#include "Engine/Graphics/3d/Base/Transform3D.h"
#include "Engine/Graphics/3d/Base/Light.h"
#include "Engine/Graphics/3d/Base/Camera3D.h"
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

public: /// ===Getter=== ///

	LightType GetLightType() const;

public:/// ===Setter=== ///
	// Material
	void SetMatiarlData(const Vector4& color, const float& shininess, const Matrix4x4& uvTransform);
	// ワールドトランスフォーム
	void SetTransformData(const Matrix4x4& WVP, const Matrix4x4& World, const Matrix4x4& WorldInverseTranspose);
	// Light
	void SetLightType(LightType type);
	// DirectionalLight
	void SetDirectionLight(const Vector4& color, const Vector3& direction, const float& intensity);
	// PointLight
	void SetPointLightData(const Vector4& color, const Vector3& position, const float& intensity, const float& radius, const float& decay);
	// SpotLight
	void SetSpotLightData(const Vector4& color, const Vector3& position, const Vector3& direction, const float& intensity, const float& distance, const float& decay, const float& cosAngle);
	// CameraForGPU
	void SetCameraForGPU(const Vector3& translate);

private:/// ===Variables(変数)=== ///

	// バッファリソース
	std::unique_ptr<Material3D> material_;
	std::unique_ptr<Transform3D> wvp_;
	std::unique_ptr<Light> directionallight_;
	std::unique_ptr<Camera3D> camera3D_;
	std::unique_ptr<Light> pointLight_;
	std::unique_ptr<Light> spotLight_;

	// バッファリソース内のデータを指すポインタ
	MaterialData3D* materialData_ = nullptr;
	TransformationMatrix3D* wvpMatrixData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	CameraForGPU* cameraForGPUData_ = nullptr;
	PointLight* pointLightData_ = nullptr;
	SpotLight* spotLightData_ = nullptr;
};

