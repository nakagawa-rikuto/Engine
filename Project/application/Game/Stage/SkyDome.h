#pragma once
#include "application/3d/Model.h"
#include <memory>

/// ====== ///
class Camera;

///=====================================================/// 
/// スカイドーム
///=====================================================///
class SkyDome {
public:

	SkyDome() = default;
	~SkyDome();

	void Initialize(const std::string& filename);
	void Update(Camera* camera);
	void Draw();

private:

	std::unique_ptr<Model> model_;
};

