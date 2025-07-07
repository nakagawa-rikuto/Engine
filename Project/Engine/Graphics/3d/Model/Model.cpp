#include "Model.h"
// c++
#include <cassert>
#include <fstream>
// Engine
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/Render.h"
#include "Engine/System/Service/CameraService.h"
// camera
#include "application/Game/Camera/Camera.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"


///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
Model::Model() = default;
Model::~Model() = default;

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
void Model::SetLightData(LightInfo light) {light_ = light; }
// 環境マップ
void Model::SetEnviromentMapData(EnviromentMapInfo enviromentMap) { enviromentMapInfo_ = enviromentMap; }

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
	vertex_ = std::make_shared<VertexBuffer3D>();
	index_ = std::make_shared<IndexBuffer3D>();
	common_ = std::make_shared<ModelCommon>();

	/// ===worldTransform=== ///
	worldTransform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
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

	/// ===カメラの設定=== ///
	camera_ = CameraService::GetActiveCamera().get();

	/// ===データの書き込み=== ///
	MateialDataWrite();
	TransformDataWrite();
	LightDataWrite();
	CameraDataWrite();
	EnviromentMapDataWrite();
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
	Render::SetGraphicsRootDescriptorTable(commandList, 3, enviromentMapInfo_.textureName);
	// 描画（Drawコール）
	commandList->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}

///-------------------------------------------/// 
/// クローン
///-------------------------------------------///
std::shared_ptr<Model> Model::Clone() const {
	std::shared_ptr<Model> clone = std::make_shared<Model>();

	clone->modelData_ = this->modelData_;
	clone->worldTransform_ = this->worldTransform_;
	clone->uvTransform_ = this->uvTransform_;
	clone->color_ = this->color_;
	clone->light_ = this->light_;
	clone->camera_ = this->camera_;

	clone->vertex_ = this->vertex_;
	clone->index_ = this->index_;
	clone->common_ = this->common_;

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
	const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
	worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);

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
	common_->SetCameraForGPU(camera_->GetTranslate());
}

///-------------------------------------------/// 
/// 環境マップの書き込み
///-------------------------------------------///
void Model::EnviromentMapDataWrite() {
	common_->SetEnviromentMapData(enviromentMapInfo_.isEnviromentMap, enviromentMapInfo_.strength);
}
