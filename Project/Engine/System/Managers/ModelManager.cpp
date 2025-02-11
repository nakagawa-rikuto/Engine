#include "ModelManager.h"
// Engine
#include "Engine/System/Service/Loader.h"
// c++
#include <fstream>

///-------------------------------------------/// 
/// ファイルの読み込み
///-------------------------------------------///
void ModelManager::Load(const std::string& directorPath, const std::string& filename) {
	// 読み込み済みモデルを検索
	if (modelDatas_.contains(filename)) {
		// 読み込み済みなら早期return
		return;
	}

	// Dataの宣言
	ModelData modeldata;
	// ベースのディレクトリパス
	const std::string& baseDirectorPath = "./Resource/Models";
	// モデル読み込み
	modeldata = LoadObjFile(baseDirectorPath + "/" + directorPath, filename);
	
	// テクスチャの読み込みとインデックス設定
	if (!modeldata.material.textureFilePath.empty()) { // 空でなければ
		// TextureManager からテクスチャを読み込み、インデックスを取得
		Loader::LoadTexture(modeldata.material.textureFilePath, modeldata.material.textureFilePath);
	}

	// モデルをMapコンテナに格納
	modelDatas_[directorPath] = modeldata;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
ModelData ModelManager::GetModelData(const std::string& directorPath) {
	assert(modelDatas_.contains(directorPath));
	return modelDatas_.at(directorPath);
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
	
	/// ===assimpでobjを読む=== ///
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュがないのは対応しない。
	/// ===meshを解析=== ///
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないMeshは今回非対応
		assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回非対応
		/// ===Meshの中身（face）を解析する=== ///
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみサポート
			/// ===Faceの中身（vertex）を解析する=== ///
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData3D vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };
				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}
			/// ===materialを解析する=== ///
			for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
				aiMaterial* material = scene->mMaterials[materialIndex];
				if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
					aiString textureFilePath;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
					modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
				}
			}
		}
	}
	/// ===assimpでNodeを解析する=== ///
	modelData.rootNode = ReadNode(scene->mRootNode);
	// ModelDataを返す
	return modelData;
}

///-------------------------------------------/// 
/// 構造体Nodeに変換するための関数
///-------------------------------------------///
Node ModelManager::ReadNode(aiNode* node) {
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose(); // 列ベクトル形式を行ベクトル形式に転置
	std::memcpy(result.localMatrix.m, &aiLocalMatrix, sizeof(aiMatrix4x4)); // aiMatrix4x4 のデータを result.localMatrix にコピー (memcpy を使用)
	result.name = node->mName.C_Str(); // Node名を格納
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}
