#pragma once
#include "application/3d/Model.h"
#include <memory>

///=====================================================/// 
/// スカイドーム
///=====================================================///
class SkyDome {
public:

	SkyDome() = default;
	~SkyDome();

	void Initialize(const std::string& filename);
	void Update();
	void Draw();

private:

	std::unique_ptr<Model> model_;
};

