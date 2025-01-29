#pragma once
/// ===Include=== ///
#include "application/3d/Model.h"
#include <memory>

/// ===前方宣言=== ///
class Camera;

///=====================================================///  
/// Goal
///=====================================================///
class Goal {
public:
	Goal() = default;
	~Goal();

	void Initialze(const std::string& modelName, const Vector3& pos, Camera* camera);
	void Update(Camera* camera);
	void Draw();

private:
	std::unique_ptr<Model> model_;
};

