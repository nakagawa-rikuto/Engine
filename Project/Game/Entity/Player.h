#pragma once
/// ===Include=== ///
#include "Game/3d/Model.h"

#include <memory>

/// ===前方宣言=== ///
class Camera;
class Block;

///=====================================================/// 
/// Player
///=====================================================///
class Player {
public:

	Player() = default;
	~Player();

	// 初期化
	void Inititalze(const std::string& modelName);
	// 更新
	void Update(Camera* camera, bool Mode);
	// 描画
	void Draw();

	// 衝突時の処理
	void OnCollision(Block* block);

public:/// ===Getter=== ///
	// ポジション
	const Vector3 GetPos();
	const float GetRadius();

public:

	void NotCollisision();

private:

	/// ===Transform情報=== ///
	Vector3 pos_;
	
	/// ===衝突判定の情報=== ///
	bool isCollision_ = false;
	float radius_;  
	float pushbackAmount_ = 0.1f; // 衝突時の押し戻し量

	/// ===Model=== ///
	std::unique_ptr<Model> model_;

private:

	// 移動処理
	void Move3D();
	void Move2D();
};

