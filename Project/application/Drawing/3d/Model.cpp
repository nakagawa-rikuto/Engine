#include "Model.h"
// Engine
#include "Engine/Core/Mii.h"
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
// camera
#include "application/Drawing/3d/Camera.h"
// Math
#include "Math/sMath.h"
// c++
#include <cassert>
#include <fstream>

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
Model::Model() = default;
Model::~Model() {
	vertex_.reset();
	common_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
/// ===モデル=== ///
const Vector3& Model::GetPosition() const { return worldTransform_.translate; }
const Vector3& Model::GetRotate() const { return worldTransform_.rotate; }
const Vector3& Model::GetScale() const { return worldTransform_.scale; }
const Vector4& Model::GetColor() const { return color_; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
/// ===モデル=== ///
void Model::SetPosition(const Vector3& position) { worldTransform_.translate = position; }
void Model::SetRotate(const Vector3& rotate) { worldTransform_.rotate = rotate; }
void Model::SetScale(const Vector3& scale) { worldTransform_.scale = scale; }
void Model::SetColor(const Vector4& color) { color_ = color; }
/// ===Light=== ///
void Model::SetLight(LightType type) { common_->SetLightType(type); }
// DirectioanlLight
void Model::SetDirectionalLight(LightInfo light, DirectionalLightInfo info) {
	light_.shininess = light.shininess;
	directional_.direction = info.direction;
	directional_.intensity = info.intensity;
	directional_.color = info.color;
}
// PointLight
void Model::SetPointLight(LightInfo light, PointLightInfo info) {
	light_.shininess = light.shininess;
	point_.position = info.position;
	point_.intensity = info.intensity;
	point_.color = info.color;
	point_.radius = info.radius;
	point_.decay = info.decay;
}
// SpotLight
void Model::SetSpotLight(LightInfo light, SpotLightInfo info) {
	light_.shininess = light.shininess;
	spot_.color = info.color;
	spot_.position = info.position;
	spot_.direction = info.direction;
	spot_.intensity = info.intensity;
	spot_.distance = info.distance;
	spot_.decay = info.decay;
	spot_.cosAngle = info.cosAngle;
}
/// ===カメラ=== ///
void Model::SetCamera(Camera* camera) { camera_ = camera; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
// オブジェクトを読み込む場合
void Model::Initialize(const std::string& filename, LightType type) {

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12Device* device = Mii::GetDXDevice();

	/// ===モデル読み込み=== ///
	modelData_ = Mii::GetModelData(filename); // ファイルパス

	/// ===生成=== ///
	vertex_ = std::make_unique<VertexBuffer3D>();
	index_ = std::make_unique<IndexBuffer3D>();
	common_ = std::make_unique<ModelCommon>();

	/// ===worldTransform=== ///
	worldTransform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
	cameraTransform_ = { {1.0f, 1.0f,1.0f}, {0.3f, 0.0f, 0.0f}, {0.0f, 4.0f, -10.0f} };
	uvTransform_ = { {1.0f, 1.0f,1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	/// ===vertex=== ///
	// Buffer
	vertex_->Create(device, sizeof(VertexData3D) * modelData_.vertices.size());
	vertex_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	// メモリコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData3D) * modelData_.vertices.size());
	// view
	vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * modelData_.vertices.size());
	vertexBufferView_.StrideInBytes = sizeof(VertexData3D);

	/// ===index=== ///
	index_->Create(device, sizeof(uint32_t) * modelData_.indices.size());
	index_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	// メモリコピー
	std::memcpy(indexData_, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
	// view
	indexBufferView_.BufferLocation = index_->GetBuffer()->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	/// ===Common=== ///
	common_->Initialize(device, type);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Model::Update() {
	/// ===データの書き込み=== ///
	MateialDataWrite();
	TransformDataWrite();
	LightDataWrite();
	CameraDataWrite();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Model::Draw(BlendMode mode) {

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12GraphicsCommandList* commandList = Mii::GetDXCommandList();

	/// ===コマンドリストに設定=== ///
	// PSOの設定
	Mii::SetPSO(commandList, PipelineType::Obj3D, mode);
	// Viewの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	// 共通部の設定
	common_->Bind(commandList);
	// テクスチャの設定
	Mii::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.material.textureFilePath);
	// 描画（Drawコール）
	commandList->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}

///-------------------------------------------/// 
/// MaterialDataの書き込み
///-------------------------------------------///
void Model::MateialDataWrite() {
	/// ===Matrixの作成=== ///
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform_.scale);
	Matrix4x4 uvTransformMatrixMultiply = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrixMultiply = Multiply(uvTransformMatrixMultiply, MakeTranslateMatrix(uvTransform_.translate));
	/// ===値の代入=== ///
	common_->SetMatiarlData(
		color_,
		light_.shininess,
		uvTransformMatrixMultiply
	);
}

///-------------------------------------------/// 
/// Transform情報の書き込み
///-------------------------------------------///
void Model::TransformDataWrite() {

	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix;

	/// ===Matrixの作成=== ///
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	} else {
		Matrix4x4 viewMatrix = Inverse4x4(MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate));
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(WinApp::GetWindowWidth()) / static_cast<float>(WinApp::GetWindowHeight()), 0.1f, 100.0f);
		worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	}
	/// ===値の代入=== ///
	common_->SetTransformData(
		worldViewProjectionMatrix,
		Multiply(modelData_.rootNode.localMatrix, worldMatrix),
		Inverse4x4(worldMatrix)
	);
}

///-------------------------------------------///  
///　ライトの書き込み
///-------------------------------------------///
void Model::LightDataWrite() {
	common_->SetDirectionLight(
		directional_.color,
		directional_.direction,
		directional_.intensity
	);
	common_->SetPointLightData(
		point_.color,
		point_.position,
		point_.intensity,
		point_.radius,
		point_.decay
	);
	common_->SetSpotLightData(
		spot_.color,
		spot_.position,
		spot_.direction,
		spot_.intensity,
		spot_.distance,
		spot_.decay,
		spot_.cosAngle
	);
}

///-------------------------------------------/// 
/// カメラの書き込み
///-------------------------------------------///
void Model::CameraDataWrite() {
	if (camera_) {
		common_->SetCameraForGPU(camera_->GetTranslate());
	} else {
		common_->SetCameraForGPU(cameraTransform_.translate);
	}
}