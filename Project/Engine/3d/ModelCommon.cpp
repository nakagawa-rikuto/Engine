#include "ModelCommon.h"
// Math
#include "Math/sMath.h"

///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
ModelCommon::ModelCommon() = default;
ModelCommon::~ModelCommon() {
	material_.reset();
	wvp_.reset();
	light_.reset();
	camera3D_.reset();
}


///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// mateial
void ModelCommon::SetMatiarlData(const Vector4& color, const float& shininess, const Matrix4x4& uvTransform) {
	materialData_->color = color;
	materialData_->shininess = shininess;
	materialData_->uvTransform = uvTransform;
}
// wvp
void ModelCommon::SetTransformData(const Matrix4x4& WVP, const Matrix4x4& World, const Matrix4x4& WorldInverseTranspose) {
	wvpMatrixData_->WVP = WVP;
	wvpMatrixData_->World = World;
	wvpMatrixData_->WorldInverseTranspose = WorldInverseTranspose;
}
// light
void ModelCommon::SetDirectionLight(const Vector4& color, const Vector3& direction, const float& intensity) {
	directionalLightData_->color = color;
	directionalLightData_->direction = direction;
	directionalLightData_->intensity = intensity;
}
// cameraForGPU
void ModelCommon::SetCameraForGPU(const Vector3& translate) {
	cameraForGPUData_->worldPosition = translate;
}


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ModelCommon::Initialize(ID3D12Device* device, LightType type) {

	/// ===生成=== ///
	material_ = std::make_unique<Material3D>();
	wvp_ = std::make_unique<Transform3D>();
	light_ = std::make_unique<Light>();
	camera3D_ = std::make_unique<Camera3D>();

	/// ===Material=== ///
	// buffer
	material_->Create(device, sizeof(MaterialData3D));
	material_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// Data書き込み
	materialData_->color = {1.0f, 1.0f, 1.0f, 1.0f};
	if (type == LightType::Lambert) {
		materialData_->enableLighting = 1;
	} else if (type == LightType::HalfLambert) {
		materialData_->enableLighting = 2;
	} else {
		materialData_->enableLighting = 0;
	}
	materialData_->shininess = 10.0f;
	materialData_->uvTransform = MakeIdentity4x4();

	/// ===wvp=== ///
	// buffer
	wvp_->Create(device, sizeof(TransformationMatrix3D));
	wvp_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&wvpMatrixData_));
	// Dataの書き込み
	wvpMatrixData_->WVP = MakeIdentity4x4();
	wvpMatrixData_->World = MakeIdentity4x4();
	wvpMatrixData_->WorldInverseTranspose = Inverse4x4(wvpMatrixData_->World);

	/// ===Light=== ///
	light_->Create(device, sizeof(DirectionalLight));
	light_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_->intensity = 1.0f;

	/// ===Camera=== ///
	camera3D_->Create(device, sizeof(CameraForGPU));
	camera3D_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPUData_));
	cameraForGPUData_->worldPosition = {0.0f, 4.0f, -10.0f};
}


///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ModelCommon::Bind(ID3D12GraphicsCommandList* commandList) {

	/// ===コマンドリストに設定=== ///
	// MaterialBufferの設定
	commandList->SetGraphicsRootConstantBufferView(0, material_->GetBuffer()->GetGPUVirtualAddress());
	// wvpMatrixBufferの設定
	commandList->SetGraphicsRootConstantBufferView(1, wvp_->GetBuffer()->GetGPUVirtualAddress());
	// Lightの設定
	commandList->SetGraphicsRootConstantBufferView(3, light_->GetBuffer()->GetGPUVirtualAddress());
	// CameraBufferの設定
	commandList->SetGraphicsRootConstantBufferView(4, camera3D_->GetBuffer()->GetGPUVirtualAddress());
}