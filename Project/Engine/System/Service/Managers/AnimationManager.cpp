#include "AnimationManager.h"
// Engine
#include "Engine/System/Service/Loader.h"
// c++
#include <fstream>

///-------------------------------------------/// 
/// ファイル読み込み
///-------------------------------------------///
void AnimationManager::Load(const std::string& baseDirectoryPath, const std::string& directorPath, const std::string& filename) {
	// 読み込み済みアニメーションを検索
	if (animationDatas_.contains(filename)) {
		// 読み込み済みなら早期return
		return;
	}

	// Dataの宣言
	Animation animationData;
	// アニメーションの読み込み
	animationData = LoadAnimation(baseDirectoryPath + "/" + directorPath, filename);

	// アニメーションをMapコンテナに格納
	animationDatas_[directorPath] = animationData;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
Animation AnimationManager::GetAnimation(const std::string& directorPath) {
	assert(animationDatas_.contains(directorPath));
	return animationDatas_.at(directorPath);
}

///-------------------------------------------/// 
/// アニメーションファイル読み込み
///-------------------------------------------///
Animation AnimationManager::LoadAnimation(const std::string& directorPath, const std::string& filename) {
	/// ===LoadAnimationを実装する=== ///
	Animation animation;  // 今回作るアニメーション
	Assimp::Importer importer;
	std::string filePath = directorPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0); // アニメーションがない
	aiAnimation* animationAssimp = scene->mAnimations[0]; // 最初のアニメーションだけ採用。もちろん複数対応するに越したことはない
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond); // 時間の単位を秒に変換

	/// ===NodeAnimationを解析する=== ///
	// assimpでは個々のNodeのAnimationをchnnelと読んでいるのでchannelを回してNodeAnimationの情報を取ってくる
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
		// Translate（キーフレーム）
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
			keyframe.value = { -keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z }; // 右手->左手
			nodeAnimation.translate.keyframes.push_back(keyframe);
		}
		// Rotate (キーフレーム) 
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuaternion keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w }; // y, z 反転
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}
		// Scale (キーフレーム) 
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z }; // スケールは変換不要
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}
	}
	// 解析完了
	return animation;
}
