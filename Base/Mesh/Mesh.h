#pragma once
/// ===include=== ///
// Window
#include <Windows.h>

// c++
#include <cstdint>
#include <d3d12.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <wrl.h>

#include "Base/Data/CBufferData.h"
#include "Base/ComPtr/ComPtr.h"

/// ===前方宣言=== ///
class DXCommon;

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
		VertexData3D* TriangleLeftBottomPositionData, VertexData3D* TriangleTopPositionData, VertexData3D* TriangleRightBottomPositionData);

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
	
	
	ComPtr<ID3D12Resource> vertexBuffer_;
	ComPtr<ID3D12Resource> indexBuffer_;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_VERTEX_BUFFER_VIEW indexBufferView_{};
	
};

