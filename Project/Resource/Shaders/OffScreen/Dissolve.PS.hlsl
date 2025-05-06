#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gMaskTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutPut
{
    float4 color : SV_TARGET0;
};

PixelShaderOutPut main(VertexShaderOutPut input)
{
    
    float mask = gMaskTexture.Sample(gSampler, input.texcoord);
    // maskの値が0.5f（聞値）以下の場合はdiscardして抜く
    if (mask <= 0.5f)
    {
        discard;
    }
    
    // Edgeっぽさを算出
    float edge = 1.0f - smoothstep(0.5f, 0.53f, mask);

    PixelShaderOutPut output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // Edgeっぽいほど指定した色を加算
    output.color.rgb += edge * float3(1.0f, 0.4f, 0.3f);
    return output;
}