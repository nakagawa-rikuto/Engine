#pragma once
/// ===Include=== ///
// C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// JSON
#include <json.hpp>
// Math
#include "Math/Vector3.h"

/// ===LevelData=== ///
struct LevelData {

	/// ===JSONObjectData=== ///
	struct JsonObjectData {
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
	};

	// vector配列
	std::vector<JsonObjectData> objects;
};

///=====================================================/// 
/// LevelLoader
///=====================================================///
class LevelLoader {
public:

	LevelLoader() = default;
	~LevelLoader() = default;

	// Load
	void LoadJSON(const std::string& file_path);

	// 再帰用関数
	void LoadobjectRecursive(nlohmann::json object, LevelData* levelData);


};

