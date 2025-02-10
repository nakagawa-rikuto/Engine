#pragma once
/// ===Include=== ///
#include "Math/Quaternion.h"
#include "Math/Vector3.h"
// c++
#include <vector>
#include <map>
#include <string>

/// <summary>
/// キーフレーム(Vector3)
/// </summary>
struct KeyframeVector3 {
	Vector3 value;     // キーフレームの値
	float time;        // キーフレームの時刻
};

/// <summary>
/// キーフレーム(Quaternion)
/// </summary>
struct KeyframeQuaternion {
	Quaternion value;  // キーフレームの値
	float time;        // キーフレームの時刻
};

/// <summary>
/// ノードアニメーション
/// </summary>
struct NodeAnimation {
	std::vector<KeyframeVector3> translate;
	std::vector<KeyframeQuaternion> rotate;
	std::vector<KeyframeVector3> scale;
};

/// <summary>
/// テンプレート(Keyframe)
/// </summary>
/// <typeparam name="tValue"></typeparam>
template <typename tValue>
struct Keyframe {
	float time;
	tValue value;
};
using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

/// <summary>
/// テンプレート(NodeAnimation)
/// </summary>
/// <typeparam name="tValue"></typeparam>
template<typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};
struct NodeAnimation {
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

struct Animation {
	float duration; // アニメーション全体の尺(単位は秒)
	// NodeAnimationの集合。Node名で引けるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations;
};