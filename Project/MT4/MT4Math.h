#pragma once
/// ===Include=== ///
// Math
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
// MT4
#include "MT4/Quaternion.h"

///=====================================================/// 
/// MT4
///=====================================================///
class MT4 {
public:

	MT4();
	~MT4();
		
	/// ===初期化=== ///
	void Initialze();
	/// ===ImGuiで描画=== ///
	void DraImgui();

	/// <summary>
	/// 任意軸回転行列の関数
	/// </summary>
	Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

	/// <summary>
	/// ある方向からある方向への回転
	/// </summary>
	Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

	/// <summary>
	/// 任意軸回転を表すQuaternionの生成
	/// </summary>
	Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	/// <summary>
	/// ベクトルをQuaternionで回転させた結果のベクトルを求める
	/// </summary>
	Vector3 RotateVector(const Vector3& cector, const Quaternion& quaternion);

	/// <summary>
	/// Quaternionから回転行列を求める
	/// </summary>
	Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	/// <summary>
	/// 球面線形補間
	/// </summary>
	Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

private:
	Quaternion rotation0;
	Quaternion rotation1;

	Quaternion interpolate0;
	Quaternion interpolate1;
	Quaternion interpolate2;
	Quaternion interpolate3;
	Quaternion interpolate4;

};





