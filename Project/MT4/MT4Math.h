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
	/// 任意軸回転行列の関数
	/// </summary>
	Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

	/// <summary>
	/// ある方向からある方向への回転
	/// </summary>
	Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

private:
	Quaternion rotate;
	Vector3 pointY;
	Matrix4x4 rotateMatrix;
	Vector3 rotateByQuaternion;
	Vector3 rotateByMatrix;
	float diffX;
	float diffY;
	float diffZ;
};





