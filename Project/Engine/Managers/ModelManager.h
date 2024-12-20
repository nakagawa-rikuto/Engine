#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
#include "Engine/Core/CData.h"
// DirectXTex
#include "DirectXTex.h"
// C++
#include <string>
#include <map>

/// ===前方宣言=== /// 
class DXCommon;

///=====================================================///  
/// モデルマネージャ
///=====================================================///
class ModelManager {
public:/// ===基本的な関数=== ///

	ModelManager() = default;
	~ModelManager() = default;

	// 初期化
	void Initialize(DXCommon* dxCommon);

public:/// ===Functions(関数)=== ///
	// .objファイルの読み込み
	void LoadModel(const std::string& directorPath, const std::string& filename);

	// モデルデータの取得
	ModelData GetModelData(const std::string& filename);

private:/// ===Variables(変数)=== ///
	// DXCommon
	DXCommon* dxCommon_ = nullptr;

	// モデルデータ
	std::map<std::string, ModelData> modelDatas_;

private:/// ===Functions(関数)=== ///
	// .mtlファイルの読み込み
	MaterialData LoadMaterialTemplateFile(const std::string& directorPath, const std::string& filename);

	// .objファイルの読み込み
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

};

