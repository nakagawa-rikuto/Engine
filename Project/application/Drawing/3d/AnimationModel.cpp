#include "AnimationModel.h"
// Engine
#include "Engine/Core/Mii.h"
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
// camera
#include "application/Drawing/3d/Camera.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"
// c++
#include <cassert>
#include <fstream>
#include <vector>

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
AnimationModel::AnimationModel() = default;
AnimationModel::~AnimationModel() {
	vertex_.reset();
	common_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
/// ===モデル=== ///
const Vector3& AnimationModel::GetTranslate() const { return worldTransform_.translate; }
const Vector3& AnimationModel::GetRotate() const { return worldTransform_.rotate; }
const Vector3& AnimationModel::GetScale() const { return worldTransform_.scale; }
const Vector4& AnimationModel::GetColor() const { return color_; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
/// ===モデル=== ///
void AnimationModel::SetTranslate(const Vector3& translate) { worldTransform_.translate = translate; }
void AnimationModel::SetRotate(const Vector3& rotate) { worldTransform_.rotate = rotate; }
void AnimationModel::SetScale(const Vector3& scale) { worldTransform_.scale = scale; }
void AnimationModel::SetColor(const Vector4& color) { color_ = color; }
/// ===Light=== ///
void AnimationModel::SetLight(LightType type) { common_->SetLightType(type); }
// DirectioanlLight
void AnimationModel::SetDirectionalLight(LightInfo light, DirectionalLightInfo info) {
	light_.shininess = light.shininess;
	directional_.direction = info.direction;
	directional_.intensity = info.intensity;
	directional_.color = info.color;
}
// PointLight
void AnimationModel::SetPointLight(LightInfo light, PointLightInfo info) {
	light_.shininess = light.shininess;
	point_.position = info.position;
	point_.intensity = info.intensity;
	point_.color = info.color;
	point_.radius = info.radius;
	point_.decay = info.decay;
}
// SpotLight
void AnimationModel::SetSpotLight(LightInfo light, SpotLightInfo info) {
	light_.shininess = light.shininess;
	spot_.color = info.color;
	spot_.position = info.position;
	spot_.direction = info.direction;
	spot_.intensity = info.intensity;
	spot_.distance = info.distance;
	spot_.decay = info.decay;
	spot_.cosAngle = info.cosAngle;
}
/// ===Camera=== ///
void AnimationModel::SetCamera(Camera* camera) { camera_ = camera; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void AnimationModel::Initialize(const std::string & filename, LightType type) {
	/// ===コマンドリストのポインタの取得=== ///
	ID3D12Device* device = Mii::GetDXDevice();

	/// ===モデル読み込み=== ///
	modelData_ = Mii::GetModelData(filename); // ファイルパス

	/// ===Animationの読み込み=== ///
	animation_ = Mii::GetAnimationData(filename); // ファイルパス

	/// ===Skeletonの作成=== ///
	skeleton_ = CreateSkeleton(modelData_.rootNode);

	/// ===生成=== ///
	vertex_ = std::make_unique<VertexBuffer3D>();
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

	/// ===Common=== ///
	common_->Initialize(device, type);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void AnimationModel::Update() {
	/// ===データの書き込み=== ///
	MateialDataWrite();
	TransformDataWrite();
	LightDataWrite();
	CameraDataWrite();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void AnimationModel::Draw(BlendMode mode) {
	/// ===コマンドリストのポインタの取得=== ///
	ID3D12GraphicsCommandList* commandList = Mii::GetDXCommandList();

	/// ===コマンドリストに設定=== ///
	// PSOの設定
	Mii::SetPSO(commandList, PipelineType::Obj3D, mode);
	// VertexBufferViewの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// 共通部の設定
	common_->Bind(commandList);
	// テクスチャの設定
	Mii::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.material.textureFilePath);
	// 描画（Drawコール）
	commandList->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

///-------------------------------------------/// 
/// MaterialDataの書き込み
///-------------------------------------------///
void AnimationModel::MateialDataWrite() {
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
void AnimationModel::TransformDataWrite() {
	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix;

	/// ===Animationの再生=== ///
	animationTime_ += 1.0f / 60.0f; // 時刻を進める。1/60で固定してあるが、計測した時間を使って可変フレーム対応する方が望ましい
	// 後々ここにif分でリピートするかしないかを選択できるようにする
	animationTime_ = std::fmod(animationTime_, animation_.duration); // 最後までいったら最初からリピート再生。リピートしなくても別にいい
	// SkeletonにAnimationを適用
	ApplyAnimation(skeleton_, animation_, animationTime_);
	// Skeletonの更新
	SkeletonUpdate(skeleton_);

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
		Multiply(skeletonSpaceMatrix_, worldViewProjectionMatrix),
		Multiply(skeletonSpaceMatrix_, worldMatrix),
		Inverse4x4(worldMatrix)
	);
}

///-------------------------------------------///  
///　ライトの書き込み
///-------------------------------------------///
void AnimationModel::LightDataWrite() {
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
void AnimationModel::CameraDataWrite() {
	if (camera_) {
		common_->SetCameraForGPU(camera_->GetTranslate());
	} else {
		common_->SetCameraForGPU(cameraTransform_.translate);
	}
}

///-------------------------------------------/// 
/// 任意の時刻の値を取得する関数
///-------------------------------------------///
Vector3 AnimationModel::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time) {
	/// ===任意の時刻の値を取得する=== ///
	assert(!keyframes.empty()); // キーがない物は返す値がわからないのでだめ
	if (keyframes.size() == 1 || time <= keyframes[0].time) { // キーが一つか、時刻がキーフレーム前なら最初の値とする
		return keyframes[0].value;
	}

	/// ===任意の自国の値を取得する2=== ///
	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を線形補間する(Lerp)
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	// ここまでできた場合は一番後の時刻よりも後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
}


///-------------------------------------------/// 
/// 任意の時刻の値を取得する関数(Quateri)
///-------------------------------------------///
Quaternion AnimationModel::CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time) {
	/// ===任意の時刻の値を取得する=== ///
	assert(!keyframes.empty()); // キーがない物は返す値がわからないのでだめ
	if (keyframes.size() == 1 || time <= keyframes[0].time) { // キーが一つか、時刻がキーフレーム前なら最初の値とする
		return keyframes[0].value;
	}

	/// ===任意の自国の値を取得する2=== ///
	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を球面線形補間する(SLerp)
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return SLerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	// ここまでできた場合は一番後の時刻よりも後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
}

///-------------------------------------------/// 
/// Nodeの階層構造からSkeletonを作る関数
///-------------------------------------------///
Skeleton AnimationModel::CreateSkeleton(const Node& rootNode) {
	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);
	
	// 名前とIndexのマッピング
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	return skeleton;
}

///-------------------------------------------/// 
/// NodeからJointを作る関数
///-------------------------------------------///
int32_t AnimationModel::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints) {
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size()); // 現在登録されている数をIndexに
	joint.parent = parent;
	joints.push_back(joint); // SkeletonのJoint列に追加
	for (const Node& child : node.children) {
		// 子Jointを作成し、そのIndexを登録
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	// 自信のIndexを返す
	return joint.index;
}

///-------------------------------------------/// 
/// Skeletonに対してAnimationの適用を行う関数
///-------------------------------------------///
void AnimationModel::ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime) {
	for (Joint& joint : skeleton.joints) {
		// 対象のJointのAnimationがあれば、値の適用を行う。下記のif分はc++17から可能になった初期化付きif分
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime);
		}
	}
}

///-------------------------------------------/// 
/// Skeletonの更新関数
///-------------------------------------------///
void AnimationModel::SkeletonUpdate(Skeleton& skeleton) {
	// 全てのJointを更新。親が若いので通常ループで処理を可能にしている。
	for (Joint& joint : skeleton.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent) { // 親がいなければ親の行列を掛ける
			joint.skeletonSpaceMatrix = Multiply(joint.localMatrix, skeleton.joints[*joint.parent].skeletonSpaceMatrix);
			skeletonSpaceMatrix_ = joint.skeletonSpaceMatrix;
		} else { // 親がいないのでlocalMatrixとskeletonSpaceMatrixは一致する
			joint.skeletonSpaceMatrix = joint.localMatrix;
			skeletonSpaceMatrix_ = joint.skeletonSpaceMatrix;
		}
	}
}
