#pragma once
/// ===include=== ///
#include "Base/Data/CBufferData.h"
#include "Base/ComPtr/ComPtr.h"

#include "Base/Buffer/Vertex/VertexBuffer2D.h"
#include "Base/Buffer/Index/IndexBuffer2D.h"
#include "Base/Buffer/Material/Material2D.h"
#include "Base/Buffer/Transform/Transform2D.h"

#include <memory>

///=====================================================/// 
/// スプライト
///=====================================================///
class Sprite {
public: /// ===基本的な関数=== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string textureFilePath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:/// ===Getter=== ///

	/// <summary>
	/// 座標
	/// </summary>
	/// <returns></returns>
	const Vector2& GetPosition() const;

	/// <summary>
	/// 回転
	/// </summary>
	/// <returns></returns>
	const float& GetRotation() const;

	/// <summary>
	/// サイズ
	/// </summary>
	/// <returns></returns>
	const Vector2& GetSize() const;

	/// <summary>
	/// 色
	/// </summary>
	/// <returns></returns>
	const Vector4& GetColor() const;

public:/// ===Setter=== ///

	/// <summary>
	/// 座標
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position);

	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="position"></param>
	void SetRotation(const float& rotation);

	/// <summary>
	/// サイズ
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size);

	/// <summary>
	/// 色
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color);

private:/// ===Variables(変数)=== ///

	// バッファリソース内のデータを指すポインタ
	VertexData2D* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	MaterialData2D* materialData_ = nullptr;
	TransformationMatrix2D* wvpMatrixData_ = nullptr;
	WorldTransform worldTransform_; 	// Transform(scale, rotate, transform)

	// バッファリソース
	std::unique_ptr<VertexBuffer2D> vertex_;
	std::unique_ptr<IndexBuffer2D> index_;
	std::unique_ptr<Material2D> material_;
	std::unique_ptr<Transform2D> wvp_;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// テクスチャ番号
	uint32_t textureIndex = 1;

	// 座標
	Vector2 position_ = { 0.0f, 0.0f };

	// 回転
	float rotation_ = 0.0f;

	// サイズ
	Vector2 size_ = { 640.0f, 360.0f };

	// 色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

private:/// ===Functions(関数)=== ///

	/// <summary>
	/// Resourceの作成関数
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateResource(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// VertexResourceへの書き込み
	/// </summary>
	void VertexDataWrite();

	/// <summary>
	/// IndexResourceへの書き込み
	/// </summary>
	void IndexDataWrite();

	/// <summary>
	/// Transform情報の書き込み
	/// </summary>
	/// <param name="transformInfo"></param>
	void TransformDataWrite();
};

