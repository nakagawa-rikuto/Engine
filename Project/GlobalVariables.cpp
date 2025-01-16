#include "GlobalVariables.h"

#include <unordered_map>

GlobalVariables* GlobalVariables::GetInstance() {

	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update() {

	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (!ImGui::BeginMenuBar()) {
		ImGui::End();
		return;
	}

	// 各グループについて
	for (auto& itGroup : datas_) {
		const std::string& groupName = itGroup.first;
		Group& group = itGroup.second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		for (auto& itItem : group.items) {
			const std::string& itemName = itItem.first;
			Item& item = itItem.second;

			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::Text("%s: %d", itemName.c_str(), *ptr);

			} else if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0.0f, 100.0f);

			} else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);

			} else if (std::holds_alternative<std::vector<int32_t>>(item.value)) {
				auto* vec = std::get_if<std::vector<int32_t>>(&item.value);
				if (vec) {
					int gridSize = 4; // グリッドのサイズ
					ImGui::Text("%s:", itemName.c_str());
					for (int z = 0; z < gridSize; ++z) {
						for (int x = 0; x < gridSize; ++x) {
							int index = z * gridSize + x;
							if (index < static_cast<int>(vec->size())) {
								ImGui::Text("%d", (*vec)[index]); // 値を表示
							}
							if (x < gridSize - 1) {
								ImGui::SameLine(); // 同じ行に表示
							}
						}
					}
				}
			}
		}
		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
}

void GlobalVariables::CreateGroup(const std::string& groupName) {

	// 指定名のオブジェクトがなければ追加する
	datas_[groupName];
}

void GlobalVariables::DisplayGrid(const std::string& windowName, const std::vector<int32_t>& values, int gridSize) {
	if (ImGui::Begin(windowName.c_str())) {
		for (int z = 0; z < gridSize; ++z) {
			for (int x = 0; x < gridSize; ++x) {
				int index = z * gridSize + x;

				if (index < static_cast<int>(values.size())) {
					ImGui::Text("%d", values[index]);
				}

				if (x < gridSize - 1) {
					ImGui::SameLine();
				}
			}
		}
		ImGui::End();
	}
}

std::vector<int32_t> GlobalVariables::CheckMissingPairs(const std::string& groupName, const std::string& key) {
	// グループの存在を確認
	if (datas_.find(groupName) == datas_.end()) {
		return {};
	}

	// キーの存在を確認
	const auto& group = datas_.at(groupName);
	if (group.items.find(key) == group.items.end()) {
		return {};
	}

	// キーがstd::vector<int32_t>型であることを確認
	const auto& item = group.items.at(key);
	if (!std::holds_alternative<std::vector<int32_t>>(item.value)) {
		return {};
	}

	// std::vector<int32_t>を取得
	const auto& values = std::get<std::vector<int32_t>>(item.value);
	std::unordered_map<int32_t, int> countMap;

	// 各値をカウント
	for (int value : values) {
		countMap[value]++;
	}

	// ペアが揃っていない値を収集
	std::vector<int32_t> missingPairs;
	for (const auto& pair : countMap) {
		if (pair.second % 2 != 0) {
			missingPairs.push_back(pair.first);
		}
	}

	return missingPairs;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目データの設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目データの設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目データの設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const std::vector<int32_t>& values) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目データの設定
	Item newItem{};
	newItem.value = values; // std::vector<int32_t>を直接格納
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}