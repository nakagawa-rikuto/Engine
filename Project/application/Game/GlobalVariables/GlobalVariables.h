#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include <ImGui.h>

#include "Math/sMath.h"

/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables {

public:
	static GlobalVariables* GetInstance();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// 配列表示
	/// </summary>
	void DisplayGrid(const std::string& windowName, const std::vector<int32_t>& values, int gridSize = 4);

	/// <summary>
	/// ペアの確認関数
	/// </summary>
	std::vector<int32_t> CheckMissingPairs(const std::string& groupName, const std::string& key);

	// 値のセット (int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	// 値のセット (float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット (Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);
	// 値のセット (std::vector)
	void SetValue(const std::string& groupName, const std::string& key, const std::vector<int32_t>& values);
	// 
	void SetValue(const std::string& groupName, const std::string& key, const std::vector<std::vector<int>>& values);

private:
	GlobalVariables() = default;                                       // コンストラクタ
	~GlobalVariables() = default;                                      // デストラクタ
	GlobalVariables(const GlobalVariables&) = delete;                  // コピーコンストラクタ
	const GlobalVariables& operator=(const GlobalVariables&) = delete; // コピー代入演算子

	// 項目
	struct Item {
		// 項目の値
		std::variant<int32_t, float, Vector3, std::vector<int32_t>, std::vector<std::vector<int32_t>>> value;
	};

	// グループ
	struct Group {
		std::map<std::string, Item> items;
	};
	// 全データ
	std::map<std::string, Group> datas_;
};