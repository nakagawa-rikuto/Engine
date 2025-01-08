#include "Block.h"

#include "Game/3d/Camera.h"

#include <cassert>
#include <iterator> // std::advance を使用するために必要

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Block::~Block() { model_.reset(); }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Block::Initialze(const std::string& modelName, const Vector3& position, Camera* camera) {
	//
	pos_ = position;

	// 生成
	model_ = std::make_unique<Model>();
	model_->Initialize(modelName);

	// モデルに位置を設定
	model_->SetPosition(pos_);
	model_->SetCamera(camera);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Block::Update(Camera* camera) {
	model_->SetCamera(camera);
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Block::Draw() {
	model_->Draw();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
const Vector3& Block::GetPos() const { return pos_; }
// サイズ
const Vector3& Block::GetSize() const { return size_; }
