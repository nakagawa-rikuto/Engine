#include "Model.h"
// c++
#include <cassert>
#include <fstream>
// Engine
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/Render.h"
// camera
#include "application/Game/Camera/Camera.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"


///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
Model::Model() = default;
Model::~Model() {
	vertex_.reset();
	index_.reset();
	common_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
/// ===モデル=== ///
const Vector3& Model::GetTranslate() const { return worldTransform_.translate; }
const Quaternion& Model::GetRotate() const { return worldTransform_.rotate; }
const Vector3& Model::GetScale() const { return worldTransform_.scale; }
const Vector4& Model::GetColor() const { return color_; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
/// ===モデル=== ///
void Model::SetTranslate(const Vector3& position) { worldTransform_.translate = position; }
void Model::SetRotate(const Quaternion& rotate) { 
	worldTransform_.rotate = rotate; 
	// 正規化を入れる
	Normalize(worldTransform_.rotate);
}
void Model::SetScale(const Vector3& scale) { worldTransform_.scale = scale; }
void Model::SetColor(const Vector4& color) { color_ = color; }
/// ===Light=== ///
void Model::SetLight(LightType type) { common_->SetLightType(type); }
// LightInfo
void Model::SetLightData(LightInfo light) {
	/*light_.shininess = light.shininess;
	if (common_->GetLightType() == LightType::Lambert ||
		common_->GetLightType() == LightType::HalfLambert) {
		light_.directional.direction = light.directional.direction;
		light_.directional.intensity = light.directional.intensity;
		light_.directional.color = light.directional.color;
	} else if (common_->GetLightType() == LightType::PointLight) {
		light_.point.position = light.point.position;
		light_.directional.intensity = light.directional.intensity;
		light_.directional.color = light.directional.color;
		light_.point.radius = light.point.radius;
		light_.point.decay = light.point.decay;
	} else if (common_->GetLightType() == LightType::SpotLight) {
		light_.spot.position = light.spot.position;
		light_.spot.direction = light.spot.direction;
		light_.spot.intensity = light.spot.intensity;
		light_.spot.color = light.spot.color;
		light_.spot.distance = light.spot.distance;
		light_.spot.decay = light.spot.decay;
		light_.spot.cosAngle = light.spot.cosAngle;
	}*/
	light_ = light;
}
/// ===カメラ=== ///
void Model::SetCamera(Camera* camera) { camera_ = camera; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
// オブジェクトを読み込む場合
void Model::Initialize(const std::string& filename, LightType type) {

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12Device* device = GraphicsResourceGetter::GetDXDevice();

	/// ===モデル読み込み=== ///
	modelData_ = GraphicsResourceGetter::GetModelData(filename); // ファイルパス

	/// ===生成=== ///
	vertex_ = std::make_unique<VertexBuffer3D>();
	index_ = std::make_unique<IndexBuffer3D>();
	common_ = std::make_unique<ModelCommon>();

	/// ===worldTransform=== ///
	worldTransform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
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
	ID3D12GraphicsCommandList* commandList = GraphicsResourceGetter::GetDXCommandList();

	/// ===コマンドリストに設定=== ///
	// PSOの設定
	Render::SetPSO(commandList, PipelineType::Obj3D, mode);
	// Viewの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	// 共通部の設定
	common_->Bind(commandList);
	// テクスチャの設定
	Render::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.material.textureFilePath);
	// 描画（Drawコール）
	commandList->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}

///-------------------------------------------/// 
/// クローン
///-------------------------------------------///
Model* Model::Clone() {
	Model* clone = new Model();
	/// ===モデルデータのクローン=== ///
	clone->modelData_ = this->modelData_;
	/// ===Transformのクローン=== ///
	clone->worldTransform_ = this->worldTransform_;
	clone->cameraTransform_ = this->cameraTransform_;
	clone->uvTransform_ = this->uvTransform_;
	/// ===カラーのクローン=== ///
	clone->color_ = this->color_;
	/// ===Lightのクローン=== ///
	clone->light_ = this->light_;
	/// ===カメラのクローン=== ///
	clone->camera_ = this->camera_;
	/// ===バッファリソースのクローン=== ///
	clone->vertex_ = std::make_unique<VertexBuffer3D>(*this->vertex_);
	clone->index_ = std::make_unique<IndexBuffer3D>(*this->index_);
	clone->common_ = std::make_unique<ModelCommon>(*this->common_);
	/// ===バッファビューのクローン=== ///
	clone->vertexBufferView_ = this->vertexBufferView_;
	clone->indexBufferView_ = this->indexBufferView_;

	return clone;
}

///-------------------------------------------/// 
/// MaterialDataの書き込み
///-------------------------------------------///
void Model::MateialDataWrite() {
	/// ===Matrixの作成=== ///
	Matrix4x4 uvTransformMatrix = Math::MakeScaleMatrix(uvTransform_.scale);
	Matrix4x4 uvTransformMatrixMultiply = Multiply(uvTransformMatrix, Math::MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrixMultiply = Multiply(uvTransformMatrixMultiply, Math::MakeTranslateMatrix(uvTransform_.translate));
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

	Matrix4x4 worldMatrix = Math::MakeAffineQuaternionMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix;

	/// ===Matrixの作成=== ///
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	} else {
		Matrix4x4 viewMatrix = Math::Inverse4x4(Math::MakeAffineEulerMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate));
		Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(0.45f, static_cast<float>(GraphicsResourceGetter::GetWindowWidth()) / static_cast<float>(GraphicsResourceGetter::GetWindowHeight()), 0.1f, 100.0f);
		worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	}
	/// ===値の代入=== ///
	common_->SetTransformData(
		worldViewProjectionMatrix,
		Multiply(modelData_.rootNode.localMatrix, worldMatrix),
		Math::Inverse4x4(worldMatrix)
	);
}

///-------------------------------------------///  
///　ライトの書き込み
///-------------------------------------------///
void Model::LightDataWrite() {
	common_->SetDirectionLight(
		light_.directional.color,
		light_.directional.direction,
		light_.directional.intensity
	);
	common_->SetPointLightData(
		light_.point.color,
		light_.point.position,
		light_.point.intensity,
		light_.point.radius,
		light_.point.decay
	);
	common_->SetSpotLightData(
		light_.spot.color,
		light_.spot.position,
		light_.spot.direction,
		light_.spot.intensity,
		light_.spot.distance,
		light_.spot.decay,
		light_.spot.cosAngle
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