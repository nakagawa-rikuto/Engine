#include "ModelManager.h"
// Engine
#include "Engine/Core/Mii.h"
// c++
#include <fstream>

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ModelManager::Initialize(DXCommon* dxCommon) {

	dxCommon_ = dxCommon;
}

///-------------------------------------------/// 
/// .objファイルの読み込み
///-------------------------------------------///
void ModelManager::LoadModel(const std::string& directorPath, const std::string& filename) {
	// 読み込み済みモデルを検索
	if (modelDatas_.contains(filename)) {
		// 読み込み済みなら早期return
		return;
	}

	ModelData modelData = LoadObjFile(directorPath + "/" + filename, filename + ".obj");

	// テクスチャの読み込みとインデックス設定
	if (!modelData.material.textureFilePath.empty()) { // 空でなければ
		// TextureManager からテクスチャを読み込み、インデックスを取得
		Mii::LoadTexture(modelData.material.textureFilePath);
	}

	// モデルをMapコンテナに格納
	modelDatas_[filename] = modelData;
}

///-------------------------------------------/// 
/// モデルデータの取得
///-------------------------------------------///
ModelData ModelManager::GetModelData(const std::string& filename) {

	assert(modelDatas_.contains(filename));
	return modelDatas_.at(filename);
}

///-------------------------------------------/// 
/// .mltファイルの読み込み
///-------------------------------------------///
MaterialData ModelManager::LoadMaterialTemplateFile(const std::string& directorPath, const std::string& filename) {
	/// ===必要な変数と宣言ファイルを開く=== ///
	MaterialData materialData; // 構築するMaterialData
	std::string line; // ファイルから読んだ1行を格納するもの
	std::ifstream file(directorPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める
	//materialData.textureFilePath = directorPath + "/" + filename; // テクスチャファイルパスを入れる

	/// ===ファイルを読み、MateiralDataを構築=== ///
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;

			// 連結してファイルパスにする
			materialData.textureFilePath = directorPath + "/" + textureFilename;
		}
	}

	// MaterialDataを返す
	return materialData;
}

ModelData ModelManager::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData; // 構築するModelData
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line; // ファイルから読んだ１行を格納するもの

	// 2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // 開けなかったら停止する

	// 3. 実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier; // 行の先頭にある識別子
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		// 頂点情報を得る
		if (identifier == "v") { // 頂点位置
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		} else if (identifier == "vt") { // 頂点テクスチャ座標
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
		} else if (identifier == "vn") { // 頂点法線
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		} else if (identifier == "f") { // 面
			VertexData3D triangle[3];

			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition; // 頂点定義
				s >> vertexDefinition;

				// 頂点の要素へのIndexは[位置/UV/法線]で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3] = { 0, 0, 0 };
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					if (std::getline(v, index, '/')) {
						if (!index.empty()) {
							elementIndices[element] = std::stoi(index);
						}
					}
				}

				// インデックスの範囲をチェックして、有効範囲内ならばその値を使用、範囲外ならデフォルト値を使用
				Vector4 position = (elementIndices[0] > 0 && elementIndices[0] <= positions.size()) ? positions[elementIndices[0] - 1] : Vector4{ 0, 0, 0, 1 };
				Vector2 texcoord = (elementIndices[1] > 0 && elementIndices[1] <= texcoords.size()) ? texcoords[elementIndices[1] - 1] : Vector2{ 0, 0 };
				Vector3 normal = (elementIndices[2] > 0 && elementIndices[2] <= normals.size()) ? normals[elementIndices[2] - 1] : Vector3{ 0, 0, 1 };

				// 位置と法線のz座標を反転する
				position.x *= -1.0f;
				normal.x *= -1.0f;
				VertexData3D vertex = { position, texcoord, normal };
				texcoord.y = 1.0f - texcoord.y;
				triangle[faceVertex] = vertex;
			}

			// 頂点を逆順で登録することで、周り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);

		} else if (identifier == "mtllib") {
			// materialTemplateLibraryファイルの名前を取得
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にObjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}

	// 4. ModelDataを返す
	return modelData;
}
