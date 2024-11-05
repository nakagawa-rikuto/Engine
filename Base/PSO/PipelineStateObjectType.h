#pragma once
/// ===include=== ///
// c++
#include <cstdint>
#include <array>

//NOTE:uint8_tを使用する理由はメモリの使用の効率がよくなるから、列挙型の値が256以上になるとUint32_tの方が良い
enum class PipelinType : uint8_t {
	// 随時追加していく
	Obj2D,
};

// パイプラインの数
constexpr uint32_t pipelineTypeNum = 1;

// パイプラインの配列
constexpr std::array<PipelinType, pipelineTypeNum> pipelineTypes = {
	PipelinType::Obj2D
};