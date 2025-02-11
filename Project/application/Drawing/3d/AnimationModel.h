#pragma once
// buffer
#include "Engine/Graphics/3d/VertexBuffer3D.h"
#include "Engine/Graphics/3d/ModelCommon.h"
// Data
#include "Engine/DataInfo/AnimationData.h"
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <memory>

/// ===前方宣言=== ///
class Camera;

///=====================================================/// 
/// アニメーションモデル
///=====================================================///
class AnimationModel {
public:

	AnimationModel();
	~AnimationModel();

	// 初期化
	void Initialize(const std::string& filename, LightType type = LightType::None);
	// 更新
	void Update();
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal);

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
	void SetTranslate(const Vector3& translate);
	void SetRotate(const Vector3& rotate);
	void SetScale(const Vector3& scale);
	// モデルColor
	void SetColor(const Vector4& color);
	// Light
	void SetLight(LightType type);
	// LightData
	void SetDirectionalLight(LightInfo shiniss, DirectionalLightInfo info);
	void SetPointLight(LightInfo shiniss, PointLightInfo info);
	void SetSpotLight(LightInfo shiniss, SpotLightInfo info);
	// Camera
	void SetCamera(Camera* camera);

private: /// ===Variables(変数)=== ///

	/// ===バッファリソース=== ///
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<ModelCommon> common_;

	/// ===バッファリソース内のデータを指すポイント=== ///
	VertexData3D* vertexData_ = nullptr;

	/// ===バッファビュー=== ///
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	/// ===モデルデータ=== ///
	ModelData modelData_;
	WorldTransform uvTransform_;

	/// ===カメラ=== ///
	Camera* camera_ = nullptr;
	WorldTransform cameraTransform_;

	/// ===モデル情報=== ///
	WorldTransform worldTransform_;
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	/// ===Light=== ///
	LightInfo light_ = { 40.0f };
	// DirctionalLight
	DirectionalLightInfo directional_ = { { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, -1.0f, 0.0f } ,1.0f };
	// PointLight
	PointLightInfo point_ = { { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f, 0.0f } , 1.0f, 0.0f, 0.0f };
	// SpotLight
	SpotLightInfo spot_ = { { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f, 0.0f } , 0.0f, { 0.0f, 0.0f, 0.0f } , 0.0f, 0.0f, 0.0f };

	/// ===Animation=== ///
	float animationTime_ = 0.0f;
	Animation animation_;

private: /// ===Functions(関数)=== ///

	// MaterialDataの書き込み
	void MateialDataWrite();
	// Transform情報の書き込み
	void TransformDataWrite();
	// LightData書き込み
	void LightDataWrite();
	// CameraData書き込み
	void CameraDataWrite();
	// 任意の時刻を取得する関数(Vector3)
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	// 任意の時刻を取得する関数(Quaternion)
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);
};

