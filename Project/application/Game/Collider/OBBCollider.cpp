#include "OBBCollider.h"
#include "Math/MatrixMath.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void OBBCollider::Initialize() {
	type_ = ColliderType::OBB;

	obb_.center = object3d_->GetTranslate();
	SetOBBAxisFromQuaternion(obb_, object3d_->GetRotate());

	// Colliderの初期化
	Collider::Initialize();

}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void OBBCollider::Update() {

	//センターの位置を更新
	obb_.center = object3d_->GetTranslate();
	// 回転によってOBBの軸を更新
	SetOBBAxisFromQuaternion(obb_, object3d_->GetRotate());

	// Colliderの更新処理
	Collider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void OBBCollider::Draw(BlendMode mode) {

	// Line
#ifdef _DEBUG
	// デバッグ時のみ描画
	line_->DrawOBB(obb_, lineColor_);

#endif // DEBUG

	// Colliderの描画処理
	Collider::Draw(mode);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
OBB OBBCollider::GetOBB() const {
	return obb_;
}

///-------------------------------------------/// 
/// OBBのAxisをQuaternionから設定
///-------------------------------------------///
void OBBCollider::SetOBBAxisFromQuaternion(OBB& obb, Quaternion rotate) {
	// 回転行列に変換
	Matrix4x4 rotMatrix = Math::MakeRotateQuaternionMatrix(rotate);

	// 各軸を行列の列ベクトルから取得
	obb.axis[0] = Vector3(rotMatrix.m[0][0], rotMatrix.m[1][0], rotMatrix.m[2][0]); // X軸
	obb.axis[1] = Vector3(rotMatrix.m[0][1], rotMatrix.m[1][1], rotMatrix.m[2][1]); // Y軸
	obb.axis[2] = Vector3(rotMatrix.m[0][2], rotMatrix.m[1][2], rotMatrix.m[2][2]); // Z軸

	// 念のため正規化（回転行列が正しければ不要）
	obb.axis[0] = Normalize(obb.axis[0]);
	obb.axis[1] = Normalize(obb.axis[1]);
	obb.axis[2] = Normalize(obb.axis[2]);
}
