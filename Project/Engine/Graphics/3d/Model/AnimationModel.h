#pragma once
// buffer
#include "Engine/Graphics/3d/Base/VertexBuffer3D.h"
#include "Engine/Graphics/3d/Base/IndexBuffer3D.h"
#include "Engine/Graphics/3d/Model/ModelCommon.h"
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
	void Initialize(const std::string& filename, LightType type);
	// 更新
	void Update();
	// 描画
	void Draw(BlendMode mode);

public: /// ===Getter=== ///
	// モデル座標
	const Vector3& GetTranslate() const;
	// モデル回転
	const Quaternion& GetRotate() const;
	// モデル拡縮
	const Vector3& GetScale() const;
	// モデルカラー
	const Vector4& GetColor() const;

public: /// ===Setter=== ///
	// モデルTransform
	void SetTranslate(const Vector3& translate);
	void SetRotate(const Quaternion& rotate);
	void SetScale(const Vector3& scale);
	// モデルColor
	void SetColor(const Vector4& color);
	// Light
	void SetLight(LightType type);
	// LightData
	void SetLightData(LightInfo light);
	// Camera
	void SetCamera(Camera* camera);
	// Animation
	void SetAnimation(const std::string& animationName, bool isLoop);

private: /// ===Variables(変数)=== ///

	/// ===バッファリソース=== ///
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<IndexBuffer3D> index_;
	std::unique_ptr<ModelCommon> common_;

	/// ===バッファリソース内のデータを指すポイント=== ///
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
	QuaternionTransform worldTransform_;
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	/// ===Light=== ///
	LightInfo light_ = {
		40.0f,
		{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, -1.0f, 0.0f } ,1.0f},
		{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f, 0.0f } , 1.0f, 0.0f, 0.0f},
		{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f, 0.0f } , 0.0f, { 0.0f, 0.0f, 0.0f } , 0.0f, 0.0f, 0.0f}
	};

	/// ===Animation=== ///
	float animationTime_;
	std::map<std::string, Animation> animation_;
	std::string animationName_;
	Skeleton skeleton_;
	SkinCluster skinCluster_;
	bool isLoop_;

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
	// Nodeの階層構造からSkeletonを作る関数
	Skeleton CreateSkeleton(const Node& rootNode);
	// NodeからJointを作る関数
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	// Skeletonに対してAnimationの適用を行う関数
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);
	// Skeletonの更新関数
	void SkeletonUpdate(Skeleton& skeleton);
	// SkinClusterの生成関数
	SkinCluster CreateSkinCluster(const ComPtr<ID3D12Device>& device, const Skeleton& skeleton, const ModelData& modelData);
	// SkinClusterの更新関数
	void SkinClusterUpdate(SkinCluster& skinCluster, const Skeleton& skeleton);
};

