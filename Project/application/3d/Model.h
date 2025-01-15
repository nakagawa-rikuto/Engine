#pragma once
/// ===include=== ///
// buffer
#include "Engine/3d/VertexBuffer3D.h"
#include "Engine/3d/ModelCommon.h"
// Pipeline
#include "Engine/Graphics/Pipeline/PipelineStateObjectType.h"
// c++
#include <memory>

/// ===前方宣言=== ///
class Camera;

///=====================================================/// 
/// モデル
///=====================================================///
class Model {
public: /// ===基本的な関数=== ///

	Model();
	~Model();

	// 初期化
	void Initialize(const std::string& filename, LightType type = LightType::None); // オブジェクトを読み込まない場合の初期化
	// 更新
	void Update();
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal);

public: /// ===Getter=== ///
	// モデル座標
	const Vector3& GetPosition() const;
	// モデル回転
	const Vector3& GetRotate() const;
	// モデル拡縮
	const Vector3& GetScale() const;
	// モデルカラー
	const Vector4& GetColor() const;
	// Lightの向き
	const Vector3& GetLightDirection() const;
	// Lightの明るさ
	const float& GetLightIntensity() const;
	// LIghtのカラー
	const Vector4& GetLightColor() const;
	// Lightの光沢度
	const float& GetShininess() const;

public: /// ===Setter=== ///
	// モデル座標
	void SetPosition(const Vector3& postion);
	// モデル回転
	void SetRotate(const Vector3 & rotate);
	// モデル拡縮
	void SetScale(const Vector3& scale);
	// モデルカラー
	void SetColor(const Vector4& color);
	// Lightの向き
	void SetLightDirection(const Vector3& direction);
	// Lightの明るさ
	void SetLightIntensity(const float& intensity);
	// Lightのカラー
	void SetLightColor(const Vector4& color);
	// Lightの光沢度
	void SetLightShininess(const float& shininess);
	// カメラ
	void SetCamera(Camera* camera);

private: /// ===Variables(変数)=== ///

	// バッファリソース
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<ModelCommon> common_;

	// バッファリソース内のデータを指すポインタ
	VertexData3D* vertexData_ = nullptr;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// モデルデータ
	ModelData modelData_;

	// WorldTransform
	WorldTransform cameraTransform_;
	WorldTransform uvTransform_;

	/// ===カメラ=== ///
	Camera* camera_ = nullptr;

	/// ===モデル情報=== ///
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };
	Vector3 rotate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	/// ===Light=== ///
	bool isLighting_ = false;
	Vector3 lightDirection_ = { 0.0f, -1.0f, 0.0f };
	float lightIntensity_ = 1.0f;
	Vector4 lightColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	float shininess_ = 0.27f;

private: /// ===Functions(関数)=== ///

	// MaterialDataの書き込み
	void MateialDataWrite();
	// Transform情報の書き込み
	void TransformDataWrite();
	// LightData書き込み
	void LightDataWrite();
	// CameraData書き込み
	void CameraDataWrite();
	
	// スフィアのデータ書き込み
	void SphereDataWrite();
};

