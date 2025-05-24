#pragma once
/// ===include=== ///
// buffer
#include "Engine/Graphics/3d/Base/VertexBuffer3D.h"
#include "Engine/Graphics/3d/Base/IndexBuffer3D.h"
#include "Engine/Graphics/3d/Model/ModelCommon.h"
// Pipeline
#include "Engine/DataInfo/PipelineStateObjectType.h"
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
	void Initialize(const std::string& filename, LightType type); // オブジェクトを読み込まない場合の初期化
	// 更新
	void Update();
	// 描画
	void Draw(BlendMode mode);

public: /// ===Getter=== ///
	// モデル座標
	const Vector3& GetTranslate() const;
	// モデル回転
	const Vector3& GetRotate() const;
	// モデル拡縮
	const Vector3& GetScale() const;
	// モデルカラー
	const Vector4& GetColor() const;

public: /// ===Setter=== ///
	// モデルTransform
	void SetTranslate(const Vector3& position);
	void SetRotate(const Vector3& rotate);
	void SetScale(const Vector3& scale);
	// モデルカラー
	void SetColor(const Vector4& color);
	// Light
	void SetLight(LightType type);
	// LightData
	void SetLightData(LightInfo light);
	// カメラ
	void SetCamera(Camera* camera);

private: /// ===Variables(変数)=== ///

	/// ===バッファリソース=== ///
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<IndexBuffer3D> index_;
	std::unique_ptr<ModelCommon> common_;

	/// ===バッファリソース内のデータを指すポインタ=== ///
	VertexData3D* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;

	/// ===バッファビュー=== ///
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	/// ===モデルデータ=== ///
	ModelData modelData_;
	EulerTransform uvTransform_;

	/// ===カメラ=== ///
	Camera* camera_ = nullptr;
	EulerTransform cameraTransform_;

	/// ===モデル情報=== ///
	EulerTransform worldTransform_;
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	/// ===Light=== ///
	LightInfo light_ = { 
		40.0f, 
		{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, -1.0f, 0.0f } ,1.0f},
		{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f, 0.0f } , 1.0f, 0.0f, 0.0f},
		{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f, 0.0f } , 0.0f, { 0.0f, 0.0f, 0.0f } , 0.0f, 0.0f, 0.0f} 
	};

private: /// ===Functions(関数)=== ///

	// MaterialDataの書き込み
	void MateialDataWrite();
	// Transform情報の書き込み
	void TransformDataWrite();
	// LightData書き込み
	void LightDataWrite();
	// CameraData書き込み
	void CameraDataWrite();
};

