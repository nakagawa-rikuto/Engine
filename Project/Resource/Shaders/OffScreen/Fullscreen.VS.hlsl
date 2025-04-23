#include "Fullscreen.hlsli"

static const uint kNumVertex = 3;
static const float4 kPositions[kNumVertex] =
{
    { -1.0f, 1.0f, 0.0f, 1.0f }, // 左上
    { 3.0f, 1.0f, 0.0f, 1.0f }, // 右上
    { -1.0f, -3.0f, 0.0f, 1.0f }, // 左下
};
static const float2 kTexCoords[kNumVertex] =
{
    { 0.0f, 0.0f }, // 左上
    { 2.0f, 0.0f }, // 右上
    { 0.0f, 2.0f }, // 左下
};

VertexShaderOutPut main(uint vertexId : SV_VertexID)
{
    VertexShaderOutPut output;
    output.position = kPositions[vertexId];
    output.texcoord = kTexCoords[vertexId];
    return output;
}