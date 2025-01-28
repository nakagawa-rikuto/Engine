#include "ModelManager.h"
// Engine
#include "Engine/Core/Mii.h"
#include "Engine/Service/Loader.h"
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
		Loader::LoadTexture(modelData.material.textureFilePath, modelData.material.textureFilePath);
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
	//std::vector<Vector4> positions; // 位置
	//std::vector<Vector3> normals; // 法線
	//std::vector<Vector2> texcoords; // テクスチャ座標
	//std::string line; // ファイルから読んだ１行を格納するもの

	// 2.ファイルを開く
	//std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	//assert(file.is_open()); // 開けなかったら停止する

	//// 3. 実際にファイルを読み、ModelDataを構築していく
	//while (std::getline(file, line)) {
	//	std::string identifier; // 行の先頭にある識別子
	//	std::istringstream s(line);
	//	s >> identifier; // 先頭の識別子を読む

	//	// 頂点情報を得る
	//	if (identifier == "v") { // 頂点位置
	//		Vector4 position;
	//		s >> position.x >> position.y >> position.z;
	//		position.w = 1.0f;
	//		positions.push_back(position);
	//	} else if (identifier == "vt") { // 頂点テクスチャ座標
	//		Vector2 texcoord;
	//		s >> texcoord.x >> texcoord.y;
	//		// y成分を反転させる
	//		texcoord.y = 1.0f - texcoord.y;
	//		texcoords.push_back(texcoord);
	//	} else if (identifier == "vn") { // 頂点法線
	//		Vector3 normal;
	//		s >> normal.x >> normal.y >> normal.z;
	//		normals.push_back(normal);
	//	} else if (identifier == "f") { // 面
	//		VertexData3D triangle[3];

	//		// 面は三角形限定。その他は未対応
	//		for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
	//			std::string vertexDefinition; // 頂点定義
	//			s >> vertexDefinition;

	//			// 頂点の要素へのIndexは[位置/UV/法線]で格納されているので、分解してIndexを取得する
	//			std::istringstream v(vertexDefinition);
	//			uint32_t elementIndices[3] = { 0, 0, 0 };
	//			for (int32_t element = 0; element < 3; ++element) {
	//				std::string index;
	//				if (std::getline(v, index, '/')) {
	//					if (!index.empty()) {
	//						elementIndices[element] = std::stoi(index);
	//					}
	//				}
	//			}

	//			// インデックスの範囲をチェックして、有効範囲内ならばその値を使用、範囲外ならデフォルト値を使用
	//			Vector4 position = (elementIndices[0] > 0 && elementIndices[0] <= positions.size()) ? positions[elementIndices[0] - 1] : Vector4{ 0, 0, 0, 1 };
	//			Vector2 texcoord = (elementIndices[1] > 0 && elementIndices[1] <= texcoords.size()) ? texcoords[elementIndices[1] - 1] : Vector2{ 0, 0 };
	//			Vector3 normal = (elementIndices[2] > 0 && elementIndices[2] <= normals.size()) ? normals[elementIndices[2] - 1] : Vector3{ 0, 0, 1 };

	//			// 位置と法線のz座標を反転する
	//			position.x *= -1.0f;
	//			normal.x *= -1.0f;
	//			VertexData3D vertex = { position, texcoord, normal };
	//			triangle[faceVertex] = vertex;
	//		}

	//		// 頂点を逆順で登録することで、周り順を逆にする
	//		modelData.vertices.push_back(triangle[2]);
	//		modelData.vertices.push_back(triangle[1]);
	//		modelData.vertices.push_back(triangle[0]);

	//	} else if (identifier == "mtllib") {
	//		// materialTemplateLibraryファイルの名前を取得
	//		std::string materialFilename;
	//		s >> materialFilename;
	//		// 基本的にObjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
	//		modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
	//	}
	//}

	//// 4. ModelDataを返す
	//return modelData;

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
	result.localMatrix.m[0][0] = aiLocalMatrix[0][0]; // 他の要素も同様に
	// ...
	result.name = node->mName.C_Str(); // Node名を格納
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}
