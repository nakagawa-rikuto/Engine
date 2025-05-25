#include "AnimationModel.h"
#define NOMINMAX
// c++
#include <Windows.h>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
// Service
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/System/Service/Getter.h"
#include "Engine/System/Service/Render.h"
// Manager
#include "Engine/System/Managers/SRVManager.h"
// camera
#include "application/Game/Camera/Camera.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"
#include "Math/MatrixMath.h"

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
AnimationModel::AnimationModel() = default;
AnimationModel::~AnimationModel() {
	vertex_.reset();
	index_.reset();
	common_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
/// ===モデル=== ///
const Vector3& AnimationModel::GetTranslate() const { return worldTransform_.translate; }
const Quaternion& AnimationModel::GetRotate() const { return worldTransform_.rotate; }
const Vector3& AnimationModel::GetScale() const { return worldTransform_.scale; }
const Vector4& AnimationModel::GetColor() const { return color_; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
/// ===モデル=== ///
void AnimationModel::SetTranslate(const Vector3& translate) { worldTransform_.translate = translate; }
void AnimationModel::SetRotate(const Quaternion& rotate) { worldTransform_.rotate = rotate; }
void AnimationModel::SetScale(const Vector3& scale) { worldTransform_.scale = scale; }
void AnimationModel::SetColor(const Vector4& color) { color_ = color; }
/// ===Light=== ///
void AnimationModel::SetLight(LightType type) { common_->SetLightType(type); }
// LightData
void AnimationModel::SetLightData(LightInfo light) { 
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
/// ===Camera=== ///
void AnimationModel::SetCamera(Camera* camera) { camera_ = camera; }
/// ===AnimatinoName=== ///
void AnimationModel::SetAnimation(const std::string& animationName, bool isLoop) { 
	animationName_ = animationName; 
	isLoop_ = isLoop;
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void AnimationModel::Initialize(const std::string & filename, LightType type) {
	/// ===コマンドリストのポインタの取得=== ///
	ID3D12Device* device = Getter::GetDXDevice();

	/// ===モデル読み込み=== ///
	modelData_ = Getter::GetModelData(filename); // ファイルパス

	/// ===Animationの読み込み=== ///
	animation_ = Getter::GetAnimationData(filename); // ファイルパス

	/*
	animation_.nodeAnimations[0] <- 0番目のノードのアニメーション
	上記のようにすることによって、ノードごとにアニメーションを管理することができる
	*/
	/// ===Boneがあれば=== ///
	if (modelData_.haveBone) {
		/// ===Skeletonの作成=== ///
		skeleton_ = CreateSkeleton(modelData_.rootNode);
		/// ===SkinClusterの作成=== ///
		skinCluster_ = CreateSkinCluster(device, skeleton_, modelData_);
	}

	/// ===生成=== ///
	vertex_ = std::make_unique<VertexBuffer3D>();
	index_ = std::make_unique<IndexBuffer3D>();
	common_ = std::make_unique<ModelCommon>();

	/// ===EulerTransform=== ///
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

	/// ===animation=== ///
	isLoop_ = true;
	animationTime_ = 0.0f;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void AnimationModel::Update() {

	/// ===Animationの再生=== ///
	float duration = animation_[animationName_].duration;
	// ループするかのif分
	if (isLoop_) {
		animationTime_ += 1.0f / 60.0f;
		animationTime_ = std::fmod(animationTime_, duration); // ループ
	} else {
		if (animationTime_ < duration) {
			animationTime_ += 1.0f / 60.0f;
			if (animationTime_ > duration) {
				animationTime_ = duration; // 明示的に止める
				// 後々ここにフラグなどを入れてもOK
			}
		}
	}
	
	// SkeletonにAnimationを適用
	ApplyAnimation(skeleton_, animation_[animationName_], animationTime_);
	// Skeletonの更新
	SkeletonUpdate(skeleton_);
	// SkinClusterの更新
	SkinClusterUpdate(skinCluster_, skeleton_);

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
	ID3D12GraphicsCommandList* commandList = Getter::GetDXCommandList();
	if (modelData_.haveBone) {
		/// ===VBVの設定=== ///
		D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
			vertexBufferView_, // VertexDataのVBV
			skinCluster_.influenceBufferView // InfluenceのVBV
		};

		/// ===コマンドリストに設定=== ///
		// PSOの設定
		Render::SetPSO(commandList, PipelineType::Skinning3D, mode);
		// Viewの設定
		commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
		commandList->IASetVertexBuffers(0, 2, vbvs);
		commandList->IASetIndexBuffer(&indexBufferView_);
		// 共通部の設定
		common_->Bind(commandList);
		// GPUを登録
		commandList->SetGraphicsRootDescriptorTable(7, skinCluster_.paletteSrvHandle.second);
		// テクスチャの設定
		Render::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.material.textureFilePath);
		// 描画（Drawコール）
		commandList->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
	} else {
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
}

///-------------------------------------------/// 
/// MaterialDataの書き込み
///-------------------------------------------///
void AnimationModel::MateialDataWrite() {
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
void AnimationModel::TransformDataWrite() {
	Matrix4x4 worldMatrix = Math::MakeAffineQuaternionMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix;

	if (modelData_.haveBone) {
		/// ===Matrixの作成=== ///
		if (camera_) {
			const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
			worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
		} else {
			Matrix4x4 viewMatrix = Math::Inverse4x4(Math::MakeAffineEulerMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate));
			Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(0.45f, static_cast<float>(Getter::GetWindowWidth()) / static_cast<float>(Getter::GetWindowHeight()), 0.1f, 100.0f);
			worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		}

		/// ===値の代入=== ///
		common_->SetTransformData(
			worldViewProjectionMatrix,
			worldMatrix,
			Math::Inverse4x4(worldMatrix)
		);
	} else {
		/// ===Animationの再生=== ///
		NodeAnimation& rootNodeAnimation = animation_[animationName_].nodeAnimations[modelData_.rootNode.name]; // rootNodeのAnimationを取得
		Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_); // 指定自国の値を取得。
		Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime_);
		Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
		Matrix4x4 localMatrix = Math::MakeAffineQuaternionMatrix(scale, rotate, translate);

		/// ===Matrixの作成=== ///
		if (camera_) {
			const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
			worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
		} else {
			Matrix4x4 viewMatrix = Math::Inverse4x4(Math::MakeAffineEulerMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate));
			Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(0.45f, static_cast<float>(Getter::GetWindowWidth()) / static_cast<float>(Getter::GetWindowHeight()), 0.1f, 100.0f);
			worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		}
		/// ===値の代入=== ///
		common_->SetTransformData(
			Multiply(localMatrix, worldViewProjectionMatrix),
			Multiply(localMatrix, worldMatrix),
			Math::Inverse4x4(worldMatrix)
		);
	}
}

///-------------------------------------------///  
///　ライトの書き込み
///-------------------------------------------///
void AnimationModel::LightDataWrite() {
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
			return Math::Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
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
			return Math::SLerp(keyframes[index].value, keyframes[nextIndex].value, t);
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
	joint.skeletonSpaceMatrix = Math::MakeIdentity4x4();
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
		joint.localMatrix = Math::MakeAffineQuaternionMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent) { // 親がいなければ親の行列を掛ける
			joint.skeletonSpaceMatrix = Multiply(joint.localMatrix, skeleton.joints[*joint.parent].skeletonSpaceMatrix);
		} else { // 親がいないのでlocalMatrixとskeletonSpaceMatrixは一致する
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

///-------------------------------------------/// 
/// SkinClusterの生成
///-------------------------------------------///
SkinCluster AnimationModel::CreateSkinCluster(
	const ComPtr<ID3D12Device>& device, const Skeleton& skeleton, const ModelData& modelData) {

	SkinCluster skinCluster;
	/// ===paletter用のResourceを確保=== ///
	uint32_t paletteIndex = ServiceLocator::GetSRVManager()->Allocate();
	skinCluster.paletteResource = CreateBufferResourceComPtr(device.Get(), sizeof(WellForGPU) * skeleton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster.mappedPalette = { mappedPalette, skeleton.joints.size() }; // spanを使ってアクセスするようにする
	skinCluster.paletteSrvHandle.first = ServiceLocator::GetSRVManager()->GetCPUDescriptorHandle(paletteIndex);
	skinCluster.paletteSrvHandle.second = ServiceLocator::GetSRVManager()->GetGPUDescriptorHandle(paletteIndex);

	/// ===Palette用のSRVを作成。StructuredBufferでアクセスできるようにする=== ///
	D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
	paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSrvDesc.Buffer.FirstElement = 0;
	paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSrvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
	paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	device->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);

	/// ===Influence用Resourceの作成=== ///
	//uint32_t influenceIndex = srvManager->Allocate();
	skinCluster.influenceResource = CreateBufferResourceComPtr(device.Get(), sizeof(VertexInfluence) * modelData.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.vertices.size()); // 0埋め。weightを0にしておく。
	skinCluster.mappedInfluence = { mappedInfluence, modelData.vertices.size() };
	// Influence用のVBVを作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);
	// InverseBindPoseMatrixを格納する場所を作成して、単位行列で埋める
	skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), Math::MakeIdentity4x4);

	/// ===ModelDataを解析してInfluenceを埋める=== ///
	for (const auto& jointWeight : modelData.skinClusterData) { // ModelのSkinClusterの情報を解析
		auto it = skeleton.jointMap.find(jointWeight.first); // jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
		if (it == skeleton.jointMap.end()) { // そんな名前のJointは存在しない。なので次に回す
			continue;
		}
		// (*it).secondにはJointのIndexが入っているので、該当のIndexのinverseBindPoseMatrixを代入
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPosematrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex]; // 該当のvertexIndexのinfluence情報を参照しておく
			for (uint32_t index = 0; index < kNumMaxInfluene; ++index) { // 空いているところに入れる
				if (currentInfluence.weights[index] == 0.0f) { // weight==0が空いている状態なので、その場所にweightとjointのindexを代入
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}

	return skinCluster;
}

///-------------------------------------------/// 
/// SkinClusterの更新関数
///-------------------------------------------///
void AnimationModel::SkinClusterUpdate(SkinCluster& skinCluster, const Skeleton& skeleton) {
	for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster.inverseBindPoseMatrices.size()); // ここで止まる
		skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix =
			Multiply(skinCluster.inverseBindPoseMatrices[jointIndex], skeleton.joints[jointIndex].skeletonSpaceMatrix);
		skinCluster.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Math::TransposeMatrix(Math::Inverse4x4(skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix));
	}
}
