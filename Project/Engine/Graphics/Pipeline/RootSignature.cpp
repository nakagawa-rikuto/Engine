#include "RootSignature.h"

#include "Engine/Core/Logger.h"
#include "Engine/Core/DXCommon.h"

#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <format>

/// <summary>
/// ルートシグネイチャの生成
/// </summary>
void RootSignature::Create(DXCommon* dxCommon, PipelineType Type) {
	HRESULT hr;

	if (Type == PipelineType::Obj2D) {
		// RootSignatureの生成
		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		/// ===DescriptroRangeの生成=== ///
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0; // 0から始める
		descriptorRange[0].NumDescriptors = 1; // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

		/// ===RootParameterの生成=== ///
		D3D12_ROOT_PARAMETER rootParameters[3]{};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号の指定

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号の指定

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		descriptionRootSignature.pParameters = rootParameters; // ルートパラメータ配列へのポインタ
		descriptionRootSignature.NumParameters = _countof(rootParameters); // 配列の長さ

		/// ===Samplerの設定=== ///
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1]{};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderRegister = 0;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// シリアライズしてバイナリにする
		hr = D3D12SerializeRootSignature(
			&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		// バイナリを元に生成
		assert(signatureBlob_);
		hr = dxCommon->GetDevice()->CreateRootSignature(
			0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
		if (FAILED(hr)) {
			assert(SUCCEEDED(hr));
		}
	} else if (Type == PipelineType::Obj3D) {
		// RootSignatureの生成
		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		/// ===DescriptroRangeの生成=== ///
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0; // 0から始める
		descriptorRange[0].NumDescriptors = 1; // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

		/// ===RootParameterの生成=== ///
		D3D12_ROOT_PARAMETER rootParameters[4] = {};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0を使う

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号0を使う

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; //CBVを使う
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[3].Descriptor.ShaderRegister = 1; // レジスタ番号0を使う

		descriptionRootSignature.pParameters = rootParameters; // ルートパラメータ配列へのポインタ
		descriptionRootSignature.NumParameters = _countof(rootParameters); // 配列の高さ

		/// ===Samplerの設定=== ///
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderRegister = 0;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// シリアライズしてバイナリにする
		hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		// バイナリを元に生成
		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
		assert(SUCCEEDED(hr));
	} else {
		return;
	}
}

/// <summary>
///  ルートシグネイチャの取得
/// </summary>
/// <returns></returns>
ID3D12RootSignature* RootSignature::GetRootSignature() const { return rootSignature_.Get(); }