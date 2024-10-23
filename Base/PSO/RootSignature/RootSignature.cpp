#include "RootSignature.h"

#include "Base/Log/Logger.h"

#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <format>

/// <summary>
/// シングルトンインスタンス
/// </summary>
RootSignature* RootSignature::GetInstance() {
	static RootSignature instance;
	return &instance;
}

/// <summary>
/// ルートシグネイチャの生成
/// </summary>
void RootSignature::Create(ID3D12Device* device) {

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_; // エラーオブジェクト
	HRESULT hr;

	// RootSignatureの生成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptroRangeの生成
	descriptorRange_[0].BaseShaderRegister = 0; // 0から始める
	descriptorRange_[0].NumDescriptors = 1; // 数は1つ
	descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameterの生成
	rootParameters_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters_[0].Descriptor.ShaderRegister = 0; // レジスタ番号の指定

	rootParameters_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
	rootParameters_[1].Descriptor.ShaderRegister = 0; // レジスタ番号の指定

	rootParameters_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters_[2].DescriptorTable.pDescriptorRanges = descriptorRange_; // Tableの中身の配列を指定
	rootParameters_[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_); // Tableで利用する数

	rootParameters_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters_[3].Descriptor.ShaderRegister = 1; // レジスタ番号1を使用する

	descriptionRootSignature.pParameters = rootParameters_; // ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters_); // 配列の長さ

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob_; // 

	// シリアライズしてバイナリにする
	hr = D3D12SerializeRootSignature(
		&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元に生成
	assert(signatureBlob_);
	hr = device->CreateRootSignature(
		0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	if (FAILED(hr)) {
		assert(SUCCEEDED(hr));
	}
}

/// <summary>
///  ルートシグネイチャの取得
/// </summary>
/// <returns></returns>
ID3D12RootSignature* RootSignature::GetRootSignature() const { return rootSignature_.Get(); }