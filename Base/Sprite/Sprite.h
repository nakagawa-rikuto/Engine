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
	const Vector2& GetPosition() const;

	/// <summary>
	/// 回転
	/// </summary>
	const float& GetRotation() const;

	/// <summary>
	/// サイズ
	/// </summary>
	const Vector2& GetSize() const;

	/// <summary>
	/// 色
	/// </summary>
	const Vector4& GetColor() const;

	/// <summary>
	/// アンカーポイント
	/// </summary>
	const Vector2& GetAnchorPoint() const;

	/// <summary>
	/// フリップ
	/// </summary>
	const bool& GetFlipX() const;
	const bool& GetFlipY() const;

	/// <summary>
	/// テクスチャ左上
	/// </summary>
	const Vector2& GetTextureLeftTop() const;

	/// <summary>
	/// テクスチャ切り出しサイズ
	/// </summary>
	const Vector2& GetTextureSize() const;

public:/// ===Setter=== ///

	/// <summary>
	/// 座標
	/// </summary>
	void SetPosition(const Vector2& position);

	/// <summary>
	/// 回転
	/// </summary>
	void SetRotation(const float& rotation);

	/// <summary>
	/// サイズ
	/// </summary>
	void SetSize(const Vector2& size);

	/// <summary>
	/// 色
	/// </summary>
	void SetColor(const Vector4& color);

	/// <summary>
	/// アンカーポイント
	/// </summary>
	void SetAnchorPoint(const Vector2& anchorPoint);

	/// <summary>
	/// フリップ
	/// </summary>
	void SetFlipX(const bool& flip);
	void SetFlipY(const bool& flip);

	/// <summary>
	/// テクスチャ左上座標
	/// </summary>
	void SetTextureLeftTop(const Vector2& textureLeftTop);

	/// <summary>
	/// テクスチャ切り出しサイズ
	/// </summary>
	void SetTextureSize(const Vector2& textureSize);

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

	// アンカーポイント
	Vector2 anchorPoint_ = { 0.0f, 0.0f };

	// 左右フリップ
	bool isFlipX_ = false;

	// 上下フリップ
	bool isFlipY_ = false;

	// テクスチャ左上
	Vector2 textureLeftTop_ = { 0.0f, 0.0f };

	// テクスチャ切り出しサイズ
	Vector2 textureSize_ = { 100.0f, 100.0f };

private:/// ===Functions(関数)=== ///

	/// <summary>
	/// Resourceの作成関数
	/// </summary>
	ComPtr<ID3D12Resource> CreateResource(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// VertexResourceへの書き込み
	/// </summary>
	void VertexDataWrite();

	/// <summary>
	/// UpdateVertexDataWrite
	/// </summary>
	void UpdateVertexDataWrite();

	/// <summary>
	/// テクスチャ範囲指定
	/// </summary>
	void SpecifyRange();

	/// <summary>
	/// IndexResourceへの書き込み
	/// </summary>
	void IndexDataWrite();

	/// <summary>
	/// Transform情報の書き込み
	/// </summary>
	void TransformDataWrite();

	/// <summary>
	/// テクスチャサイズをイメージに合わせる
	/// </summary>
	void AdjustTextureSize();
};

