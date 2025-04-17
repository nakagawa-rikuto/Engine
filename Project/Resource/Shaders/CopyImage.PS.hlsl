#include "CopyImage.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutPut 
{
    float4 color : SV_TARGET0;
};

PixelShaderOutPut main(VertexShaderOutPut input) 
{
    PixelShaderOutPut output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    return output;
}