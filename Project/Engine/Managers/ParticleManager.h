#pragma once
/// ===Include=== ///
#include "Engine/Core/ComPtr.h"
#include "Engine/Core/CData.h"

///=====================================================/// 
/// パーティクルマネージャ
///=====================================================///
class ParticleManager {
public:

	ParticleManager();
	~ParticleManager();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

private:

	struct  ParticleGrop {
		MaterialData material;
	};

};

