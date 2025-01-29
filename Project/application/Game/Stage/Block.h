#pragma once
/// ===Include=== ///
#include "application/3d/Model.h"
 
#include <memory>

/// ===前方宣言=== ///
class Camera;

///=====================================================/// 
/// Block
///=====================================================///
class Block {
public:
	Block() = default;
	~Block();

	void Initialze(const std::string& modelName, const Vector3& position, Camera* camera);
	void Update(Camera* camera);
	void Draw();

	const Vector3& GetPos()const;
	const Vector3& GetSize()const;

private:
	/// ===Transform情報=== ///
	Vector3 pos_;
	Vector3 size_ = { 8.0f, 8.0f, 8.0f };

	std::unique_ptr<Model> model_;
};

