#pragma once
/// ===include=== ///
#include "Base/Data/CBufferData.h"
#include "Base/ComPtr/ComPtr.h"

#include "Base/Buffer/Vertex/VertexBuffer3D.h"
#include "Base/Buffer/Index/IndexBuffer3D.h"
#include "Base/Buffer/Material/Material3D.h"
#include "Base/Buffer/Transform/Transform3D.h"

#include "Base/PSO/PipelineStateObjectCommon.h"
#include "Base/PSO/PipelineStateObjectType.h"

#include <memory>

///=====================================================/// 
/// モデル
///=====================================================///
class Model {
public: /// ===基本的な関数=== ///

	Model();
	~Model();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="mode"></param>
	void Initialize(BlendMode mode = BlendMode::KBlendModeNormal);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: /// ===Getter=== ///

public: /// ===Setter=== ///

private: /// ===Variables(変数)=== ///

private: /// ===Functions(関数)=== ///
};

