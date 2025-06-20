#include "SkyBox.h"
// c++
#include <cassert>
#include <fstream>
// Engine
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/Render.h"
// camera
#include "application/Game/Camera/camera.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
SkyBox::~SkyBox() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SkyBox::Initialize(const std::string & fileName, LightType type) {
	/// ===コマンドリストのポインタの取得=== ///
	ID3D12Device* device = GraphicsResourceGetter::GetDXDevice();

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
	// 頂点情報の設定
	VertexDataWrite();

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
void SkyBox::Update() {
	/// ===データの書き込み=== ///
	VertexDataWrite();
	MateialDataWrite();
	TransformDataWrite();
	LightDataWrite();
	CameraDataWrite();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SkyBox::Draw(BlendMode mode) {
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
/// VertrexDataの書き込み
///-------------------------------------------///
void SkyBox::VertexDataWrite() {
	// 右面。描画インデックスは[0,1,2][2,1,3]で内側を向く
	vertexData_[0].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[1].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[2].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[3].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	// 左面。描画インデックスは[4,5,6][6,5,7]
	vertexData_[4].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[5].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[6].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[7].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	// 前面。描画インデックスは[8,9,10][10,9,11]
	vertexData_[8].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[9].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[10].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[11].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	// 後面
	vertexData_[12].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[13].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[14].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[15].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	// 上面
	vertexData_[16].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[17].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[18].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[19].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 下面
	vertexData_[20].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[21].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[22].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[23].position = { 1.0f, -1.0f, -1.0f, 1.0f };
}

///-------------------------------------------/// 
/// MaterialDataの書き込み
///-------------------------------------------///
void SkyBox::MateialDataWrite() {
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
/// WVPDataの書き込み
///-------------------------------------------///
void SkyBox::TransformDataWrite() {
	Matrix4x4 worldMatrix = Math::MakeAffineEulerMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
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
/// LightDataの書き込み
///-------------------------------------------///
void SkyBox::LightDataWrite() {
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
/// CameraDataの書き込み
///-------------------------------------------///
void SkyBox::CameraDataWrite() {
	if (camera_) {
		common_->SetCameraForGPU(camera_->GetTranslate());
	} else {
		common_->SetCameraForGPU(cameraTransform_.translate);
	}
}
