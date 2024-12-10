#include "Model.h"
// Engine
#include "Engine/Core/Mii.h"
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
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
	common_.reset();
}


///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// 座標
const Vector3& Model::GetPosition() const { return position_; }
// 回転
const Vector3& Model::GetRotate() const { return rotate_; }
// 拡縮
const Vector3& Model::GetScale() const { return scale_; }
// カラー
const Vector4& Model::GetColor() const { return color_; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// 座標
void Model::SetPosition(const Vector3& postion) { position_ = postion; }
// 回転
void Model::SetRotate(const Vector3& rotate) { rotate_ = rotate; }
// 拡縮
void Model::SetScale(const Vector3& scale) { scale_ = scale; }
// カラー
void Model::SetColor(const Vector4& color) { color_ = color; }
// カメラ
void Model::SetCamera(Camera* camera) { camera_ = camera; }


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
// オブジェクトを読み込む場合
void Model::Initialize(const std::string& filename) {

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12Device* device = Mii::GetDXDevice();

	/// ===モデル読み込み=== ///
	modelData_ = Mii::GetModelData(filename); // ファイルパス

	/// ===生成=== ///
	common_ = std::make_unique<ModelCommon>();

	/// ===vertex=== ///
	common_->VertexInitialize(device, modelData_);
	/// ===Material=== ///
	common_->MaterialInitialize(device);
	/// ===wvp=== ///
	common_->WVPMatrixInitialize(device);
	/// ===Light=== ///
	common_->LightInitialize(device);

	/// ===worldTransform=== ///
	worldTransform_ = { {1.0f, 1.0f,1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	cameraTransform_ = { {1.0f, 1.0f,1.0f}, {0.3f, 0.0f, 0.0f}, {0.0f, 4.0f, -10.0f} };
	uvTransform_ = { {1.0f, 1.0f,1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
}


///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Model::Update() {
	LightDataWrite();
	TransformDataWrite();
}


///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Model::Draw(BlendMode mode) {
	/// ===コマンドリストのポインタの取得=== ///
	ID3D12GraphicsCommandList* commandList = Mii::GetDXCommandList();

	/// ===コマンドリストに設定=== ///
	common_->Bind(commandList, PipelineType::Obj3D, mode);
	// テクスチャの設定
	Mii::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.material.textureFilePath);
	// 描画（Drawコール）
	commandList->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

///-------------------------------------------///  
///　ライトの書き込み
///-------------------------------------------///
void Model::LightDataWrite() {
	DirectionalLight lightData{};
	lightData.color = lightColor_;
	lightData.direction = direction_;
	lightData.intensity = intensity_;
	common_->SetLightData(lightData);
}

///-------------------------------------------/// 
/// スフィアのデータ書き込み
///-------------------------------------------///
/*void Model::SphereDataWrite() {

	// 経度,緯度
	const float kLatEvery = Pi() / float(kSubdivision_); // 経度
	const float kLonEvery = Pi() * 2.0f / float(kSubdivision_); // 緯度

	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision_; ++latIndex) {
		float lat = -Pi() / 2.0f + kLatEvery * latIndex; // θ
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision_; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision_ + lonIndex) * 6;
			float lon = lonIndex * kLonEvery; //


			// 頂点データを入力
			// a
			vertexData_[start].position.x = std::cos(lat) * std::cos(lon);
			vertexData_[start].position.y = std::sin(lat);
			vertexData_[start].position.z = std::cos(lat) * std::sin(lon);
			vertexData_[start].position.w = 1.0f;
			vertexData_[start].texcoord = { float(lonIndex) / float(kSubdivision_), 1.0f - float(latIndex) / float(kSubdivision_) };
			vertexData_[start].normal.x = vertexData_[start].position.x;
			vertexData_[start].normal.y = vertexData_[start].position.y;
			vertexData_[start].normal.z = vertexData_[start].position.z;

			// b
			vertexData_[start + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData_[start + 1].position.y = std::sin(lat + kLatEvery);
			vertexData_[start + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData_[start + 1].position.w = 1.0f;
			vertexData_[start + 1].texcoord = { float(lonIndex) / float(kSubdivision_), 1.0f - float(latIndex + 1) / float(kSubdivision_) };
			vertexData_[start + 1].normal.x = vertexData_[start + 1].position.x;
			vertexData_[start + 1].normal.y = vertexData_[start + 1].position.y;
			vertexData_[start + 1].normal.z = vertexData_[start + 1].position.z;

			// c
			vertexData_[start + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData_[start + 2].position.y = std::sin(lat);
			vertexData_[start + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData_[start + 2].position.w = 1.0f;
			vertexData_[start + 2].texcoord = { float(lonIndex + 1) / float(kSubdivision_), 1.0f - float(latIndex) / float(kSubdivision_) };
			vertexData_[start + 2].normal.x = vertexData_[start + 2].position.x;
			vertexData_[start + 2].normal.y = vertexData_[start + 2].position.y;
			vertexData_[start + 2].normal.z = vertexData_[start + 2].position.z;

			// d
			vertexData_[start + 3].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexData_[start + 3].position.y = std::sin(lat + kLatEvery);
			vertexData_[start + 3].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexData_[start + 3].position.w = 1.0f;
			vertexData_[start + 3].texcoord = { float(lonIndex + 1) / float(kSubdivision_), 1.0f - float(latIndex + 1) / float(kSubdivision_) };
			vertexData_[start + 3].normal.x = vertexData_[start + 3].position.x;
			vertexData_[start + 3].normal.y = vertexData_[start + 3].position.y;
			vertexData_[start + 3].normal.z = vertexData_[start + 3].position.z;

			// Create two triangles (6 indices) for the current quad
			uint32_t baseIndex = (latIndex * kSubdivision_ + lonIndex) * 6;
			indexData_[baseIndex] = start;
			indexData_[baseIndex + 1] = start + 1;
			indexData_[baseIndex + 2] = start + 2;
			indexData_[baseIndex + 3] = start + 2;
			indexData_[baseIndex + 4] = start + 1;
			indexData_[baseIndex + 5] = start + 3;
		}
	}
}*/

///-------------------------------------------/// 
/// Transform情報の書き込み
///-------------------------------------------///
void Model::TransformDataWrite() {

	/// ===データの書き込み=== ///
	worldTransform_.scale = scale_;
	worldTransform_.rotate = rotate_;
	worldTransform_.translate = position_;

	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix;

	/// ===Matrixの作成=== ///
	// wvp
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	} else {
		Matrix4x4 viewMatrix = Inverse4x4(MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate));
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(WinApp::GetWindowWidth()) / static_cast<float>(WinApp::GetWindowHeight()), 0.1f, 100.0f);
		worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	}
	// UV
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform_.scale);
	Matrix4x4 uvTransformMatrixMultiply = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrixMultiply = Multiply(uvTransformMatrixMultiply, MakeTranslateMatrix(uvTransform_.translate));

	/// ===値の代入=== ///
	// wvp
	common_->SetWVPData(worldViewProjectionMatrix, worldMatrix);
	// uv
	MaterialData3D material = {};
	material.color = color_;
	//material.enableLighting = 0;
	material.uvTransform = uvTransformMatrixMultiply;
	common_->SetMaterialData(material);
}