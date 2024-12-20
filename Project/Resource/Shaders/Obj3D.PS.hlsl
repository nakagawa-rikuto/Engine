#include "Obj3D.hlsli"

struct Material
{
    float4 color;
    int enableLighting; // 0はライト無し、1はランバート、2はハーフランバート
    float4x4 uvTransform;
};

struct DirectionalLight
{
    float4 color; // ライトの色
    float3 direction; // ライトの向き
    float intensity; // ライトの明るさ(輝度)
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct PixlShaderOutput
{
    float4 color : SV_TARGET0;
};

//SRVのregisterはt
Texture2D<float4> gTexture : register(t0);

// Samplerのregisterはs
SamplerState gSampler : register(s0);


PixlShaderOutput main(VertexShaderOutput input)
{
    PixlShaderOutput output;
    output.color = gMaterial.color;
    
    float4 transformdUV = mul(float4(input.texcood, 0.0f, 1.0f), gMaterial.uvTransform);
    
    //TextureをSamplingする
    float4 textureColor = gTexture.Sample(gSampler, transformdUV.xy);
    
    // textureのa値が0.5以下の時にPixelを棄却
    if (textureColor.a <= 0.5f)
    {
        discard;
    }
    
    // textureのa値が0の時にPixelを棄却
    if (textureColor.a == 0.0f)
    {
        discard;
    }
    
    // output.colorのa値が0の時にPixelを棄却
    if (output.color.a == 0.0f)
    {
        discard;
    }
    
    // Samplingしたtextureの色とmaterialの色を乗算して合成っしてみる
    output.color = gMaterial.color * textureColor;
    
    if (gMaterial.enableLighting == 1) { // Lightingする場合
        
        /* Lambert */
        float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    else if (gMaterial.enableLighting == 2) {
        /* Half Lambert */
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    else { // Lightingしない場合。前回までと同じ演算
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}