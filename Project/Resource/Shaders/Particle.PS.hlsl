#include "Particle.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct PixlShaderOutput
{
    float4 color : SV_TARGET0;
};

//SRV��register��t
Texture2D<float4> gTexture : register(t0);

// Sampler��register��s
SamplerState gSampler : register(s0);


PixlShaderOutput main(VertexShaderOutput input)
{
    PixlShaderOutput output;
    
    // TransformedUV�̌v�Z
    float4 transformedUV = mul(float4(input.texcood, 0.0f, 1.0f), gMaterial.uvTransform);
    
    //Texture��Sampling����
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    // Color�̌v�Z
    output.color = gMaterial.color * textureColor * input.color;
    
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    return output;
}