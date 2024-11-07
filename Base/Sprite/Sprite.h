#pragma once
/// ===include=== ///
#include "Base/Data/CBufferData.h"
#include "Base/ComPtr/ComPtr.h"

#include "Base/Buffer/Vertex/VertexBuffer2D.h"
#include "Base/Buffer/Index/IndexBuffer2D.h"
#include "Base/Buffer/Material/Material2D.h"
#include "Base/Buffer/Transform/Transform2D.h"

#include "Base/PSO/PipelineStateObjectCommon.h"
#include "Base/PSO/PipelineStateObjectType.h"

#include <memory>

///=====================================================/// 
/// スプライト
///=====================================================///
class Sprite {
public: /// ===基本的な関数=== ///

	Sprite();
	~Sprite();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(BlendMode mode = BlendMode::KBlendModeNormal);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:/// ===Getter=== ///

	// 座標
	const Vector2& GetPosition() const;
	// 回転
	const float& GetRotation() const;
	// サイズ
	const Vector2& GetSize() const;
	// 色
	const Vector4& GetColor() const;

public:/// ===Setter=== ///

	// 座標
	void SetPosition(const Vector2& position);
	// 回転
	void SetRotation(const float& rotation);
	// サイズ
	void SetSize(const Vector2& size);
	// 色
	void SetColor(const Vector4& color);
	// アンカーポイント
	void SetAnchorPoint(const Vector2& anchorPoint);
	// フリップ
	void SetFlipX(const bool& flip);
	void SetFlipY(const bool& flip);
	// テクスチャ
	void SetTexture(std::string textureFilePath);
	// テクスチャ左上座標
	void SetTextureLeftTop(const Vector2& textureLeftTop);
	// テクスチャ切り出しサイズ
	void SetTextureSize(const Vector2& textureSize);

private:/// ===Variables(変数)=== ///

	// バッファリソース
	std::unique_ptr<VertexBuffer2D> vertex_;
	std::unique_ptr<IndexBuffer2D> index_;
	std::unique_ptr<Material2D> material_;
	std::unique_ptr<Transform2D> wvp_;

	// パイプライン
	std::unique_ptr<PipelineStateObjectCommon> pipelineCommon_;

	// バッファリソース内のデータを指すポインタ
	VertexData2D* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	MaterialData2D* materialData_ = nullptr;
	TransformationMatrix2D* wvpMatrixData_ = nullptr;

	// WorldTransform
	WorldTransform worldTransform_; 	// Transform(scale, rotate, transform)

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	/// ===テクスチャ情報=== ///
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

