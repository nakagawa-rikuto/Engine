#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <wrl.h>

#include "VectorData.h"
#include "CBufferData.h"
#include "DXCommon.h"

class Mesh {

public:
	
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static Mesh* GetInstance();

	/// <summary>
	/// VertexResourceの生成
	/// </summary>
	void CreateVertexResource(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// 三角形のデータの書き込み
	/// </summary>
	/// <param name="TriangleLeftBottomPositionData"></param>
	/// <param name="TriangleTopPositionData"></param>
	/// <param name="TriangleRightBottomPositionData"></param>
	void WriteTriangleData(
		VertexData* TriangleLeftBottomPositionData, VertexData* TriangleTopPositionData, VertexData* TriangleRightBottomPositionData);

	/// <summary>
	/// VertexBufferの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetVertexBuffer();

	/// <summary>
	/// VertexBufferViewの取得
	/// </summary>
	/// <returns></returns>
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();

private:

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_VERTEX_BUFFER_VIEW indexBufferView_{};
	
};

