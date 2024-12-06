#pragma once
/// ===Include=== ///
#include "Game/Scene/IScene.h"
#include <memory>
#include <string>

///=====================================================/// 
/// シーン工場（概念）
///=====================================================///
class AbstractSceneFactory {
public:
	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	// シーン生成
	virtual std::unique_ptr<IScene> CreateScene(const std::string& sceneName) = 0;
};

