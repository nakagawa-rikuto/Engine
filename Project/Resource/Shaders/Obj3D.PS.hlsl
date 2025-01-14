#include "Obj3D.hlsli"
// マテリアル
struct Material
{
    float4 color;
    int enableLighting; // 0はライト無し、1はランバート、2はハーフランバート
    float4x4 uvTransform;
    float shininess;  // 光沢度
};
// ダイレクショナルライト
struct DirectionalLight
{
    float4 color; // ライトの色
    float3 direction; // ライトの向き
    float intensity; // ライトの明るさ(輝度)
};
// カメラ
struct Camera
{
    float3 worldPosition; // カメラの位置
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);

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
    
    // textureのa値が0.5以下の時またはoutput.colorのa値が0の時にPixelを棄却
    if (textureColor.a <= 0.5f || output.color.a == 0.0f)
    {
        discard;
    }

    
    // ライトの有無
    if (gMaterial.enableLighting != 0) { // ライト有り
       // diffuseFactorの宣言
        float diffuseFactor = 0.0f;
        if (gMaterial.enableLighting == 1) {
            
             /* Lambert */
            diffuseFactor = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        }
        else if (gMaterial.enableLighting == 2) {
            
             /* Half Lambert */
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            diffuseFactor = pow(NdotL * 0.5f + 0.5f, 2.0f);
        }
        
        // カメラへの方向を算出
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        // 入射光の反射ベクトル
        float3 reflectLight = reflect(normalize(gDirectionalLight.direction), normalize(input.normal));
        // 内積の計算
        float RdotE = dot(reflectLight, toEye);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
        // 拡散反射
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * diffuseFactor * gDirectionalLight.intensity;
        // 鏡面反射
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f); // 物体の鏡面反射色は白。
        // 拡散反射・鏡面反射
        output.color.rgb = diffuse + specular;
        // アルファ今まで通り
        output.color.a = gMaterial.color.a * textureColor.a;
    } 
    else {　//　ライト無し
        
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}