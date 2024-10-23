
#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

class RootSignature {

public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static RootSignature* GetInstance();

	/// <summary>
	/// ルートシグネイチャの生成
	/// </summary>
	void Create(ID3D12Device* device);

	/// <summary>
	/// ルートシグネイチャの取得
	/// </summary>
	/// <returns></returns>
	ID3D12RootSignature* GetRootSignature()const;



private:

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_; // ルートシグネイチャ
	D3D12_ROOT_PARAMETER rootParameters_[4]{};
	D3D12_DESCRIPTOR_RANGE descriptorRange_[1]{};

};
