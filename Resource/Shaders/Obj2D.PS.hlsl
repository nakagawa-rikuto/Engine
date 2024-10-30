#include "Obj2D.hlsli"

struct Material
{
    float4 color;
    float4x4 uvTransform;
};

ConstantBuffer<Material> gMaterial : register(b0);
//SRV‚Ìregister‚Ít
Texture2D<float4> gTexture : register(t0);
// Sampler‚Ìregister‚Ís
SamplerState gSampler : register(s0);

struct PixlShaderOutput
{
    float4 color : SV_TARGET0;
};

PixlShaderOutput main(VertexShaderOutput input)
{
    PixlShaderOutput output;
    
    float4 transformdUV = mul(float4(input.texcood, 0.0f, 1.0f), gMaterial.uvTransform);
    
    //Texture‚ğSampling‚·‚é
    float4 textureColor = gTexture.Sample(gSampler, transformdUV.xy);
    
    // Sampling‚µ‚½texture‚ÌF‚Æmaterial‚ÌF‚ğæZ‚µ‚Ä‡¬‚Á‚µ‚Ä‚İ‚é
    output.color = gMaterial.color * textureColor;
    
    return output;
}