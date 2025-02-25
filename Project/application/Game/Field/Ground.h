#pragma once
#include "application/Drawing/3d/Model.h"
#include "application/Drawing/3d/Camera.h"

///-------------------------------------------/// 
/// Ground
///-------------------------------------------///
class Ground {
public:/// ===メンバ関数=== ///
	Ground() = default;
	~Ground();
	// 初期化
	void Initialize(const std::string modelname);
	// 更新
	void Update(Camera* camera);
	// 描画
	void Draw();

private:/// ===メンバ変数=== ///
	std::unique_ptr<Model> model_;
};

