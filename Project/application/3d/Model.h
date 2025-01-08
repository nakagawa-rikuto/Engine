#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/CData.h"
#include "Engine/Core/ComPtr.h"
#include "Engine/3d/VertexBuffer3D.h"
#include "Engine/3d/IndexBuffer3D.h"
#include "Engine/3d/Material3D.h"
#include "Engine/3d/Transform3D.h"
#include "Engine/Graphics/Light.h"
#include "Engine/Graphics/Pipeline/PipelineStateObjectCommon.h"
// Game
#include "application/3d/Camera.h"
#include "Engine/Graphics/Pipeline/PipelineStateObjectType.h"
// c++
#include <memory>

///=====================================================/// 
/// モデル
///=====================================================///
class Model {
public: /// ===基本的な関数=== ///

	Model();
	~Model();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& filename); // オブジェクトを読み込まない場合の初期化

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal);

public: /// ===Getter=== ///
	// 座標
	const Vector3& GetPosition() const;
	// 回転
	const Vector3& GetRotate() const;
	// 拡縮
	const Vector3& GetScale() const;
	// カラー
	const Vector4& GetColor() const;

public: /// ===Setter=== ///
	// 座標
	void SetPosition(const Vector3& postion);
	// 回転
	void SetRotate(const Vector3 & rotate);
	// 拡縮
	void SetScale(const Vector3& scale);
	// カラー
	void SetColor(const Vector4& color);
	// カメラ
	void SetCamera(Camera* camera);

private: /// ===Variables(変数)=== ///

	// バッファリソース
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<Material3D> material_;
	std::unique_ptr<Transform3D> wvp_;
	std::unique_ptr<Light> light_;

	// バッファリソース内のデータを指すポインタ
	VertexData3D* vertexData_ = nullptr;
	MaterialData3D* materialData_ = nullptr;
	TransformationMatrix3D* wvpMatrixData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// モデルデータ
	ModelData modelData_;

	// WorldTransform
	WorldTransform worldTransform_; // Transform(scale, rotate, transform)
	WorldTransform cameraTransform_;
	WorldTransform uvTransform_;

	/// ===カメラ=== ///
	Camera* camera_ = nullptr;

	/// ===モデル情報=== ///
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };
	Vector3 rotate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

private: /// ===Functions(関数)=== ///

	/// <summary>
	/// LightData書き込み
	/// </summary>
	void LightDataWrite();

	/// <summary>
	/// スフィアのデータ書き込み
	/// </summary>
	void SphereDataWrite();

	/// <summary>
	/// Transform情報の書き込み
	/// </summary>
	void TransformDataWrite();
};

