#include "Obj3D.hlsli"
// マテリアル
struct Material
{
    float4 color;
    int enableLighting; // 0はライト無し、1はランバート、2はハーフランバート
    float4x4 uvTransform;
    float shininess; // 光沢度
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
// PointLight
struct PointLight
{
    float4 color;    // ライトの色
    float3 position; // ライトの位置
    float intensity; // 輝度
    float radius;    // ライトの届く最大距離
    float decay;     // 減衰率
};
// SpotLight
struct SpotLight
{
    float4 color;     // ライトの色
    float3 position;  // ライトの位置
    float intensity;  // 輝度
    float3 direction; // スポットライトの方向
    float distance;   // ライトの届く最大距離
    float decay;      // 減衰率
    float cosAngle;   // スポットライトの余弦
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
ConstantBuffer<SpotLight> gSpotLight : register(b4);

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
    
    // UV
    float4 transformdUV = mul(float4(input.texcood, 0.0f, 1.0f), gMaterial.uvTransform);
    // TextureをSamplingする
    float4 textureColor = gTexture.Sample(gSampler, transformdUV.xy);
    // カメラへの方向を算出
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    // 入射光を計算
    float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
    float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
    
    output.color = gMaterial.color * textureColor;
    // textureのa値が0の時にPixelを棄却
    if (output.color.a == 0.0f)
    {
        discard;
    }
    
    // Lighting
    if (gMaterial.enableLighting != 0) // Lightingする場合
    { 
       // 変数の宣言
        float diffuseFactor = 0.0f;
        float specularPow = 0.0f;
        float RdotE = 0.0f;
        float3 reflectLight = { 0.0f, 0.0f, 0.0f };
        float3 diffuseDirectionalLight = { 0.0f, 0.0f, 0.0f };
        float3 specularDirectionalLight = { 0.0f, 0.0f, 0.0f };
        float3 diffusePointLight = { 0.0f, 0.0f, 0.0f };
        float3 specularPointLight = { 0.0f, 0.0f, 0.0f };
        float3 diffuseSpotLight = { 0.0f, 0.0f, 0.0f };
        float3 specularSpotLight = { 0.0f, 0.0f, 0.0f };
        float3 normal = normalize(input.normal);
        if (gMaterial.enableLighting == 1)/* Lambert */
        {
            diffuseFactor = saturate(dot(normal, -gDirectionalLight.direction));
            // 入射光の反射ベクトル
            reflectLight = reflect(normalize(gDirectionalLight.direction), normal);
            // 内積の計算
            RdotE = dot(reflectLight, toEye);
            specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
            diffuseDirectionalLight = 
            gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * diffuseFactor * gDirectionalLight.intensity;
            specularDirectionalLight = 
            gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f); // 物体の鏡面反射色は白。
        }
        else if (gMaterial.enableLighting == 2)/* Half Lambert */
        {
            float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
            float NdotH = dot(normal, halfVector);
            diffuseFactor = pow(NdotH * 0.5f + 0.5f, 2.0f);
            specularPow = pow(saturate(NdotH), gMaterial.shininess);
            diffuseDirectionalLight = 
            gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * diffuseFactor * gDirectionalLight.intensity;
            specularDirectionalLight = 
            gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f); // 物体の鏡面反射色は白。
        }
        else if (gMaterial.enableLighting == 3)/* PointLight */
        {
            diffuseFactor = saturate(dot(normal, -pointLightDirection));
            // 入射光の反射ベクトル
            reflectLight = reflect(-pointLightDirection, normal);
            // 内積の計算 
            RdotE = dot(reflectLight, toEye);
            specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
            // 距離による減衰 (1 / 距離の2乗)
            float distance = length(gPointLight.position - input.worldPosition);  // ポイントライトへの距離
            float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);  // 指数によるコントロール
            diffusePointLight = 
            gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * diffuseFactor * gPointLight.intensity * factor;
            specularPointLight = 
            gPointLight.color.rgb * gPointLight.intensity * specularPow * factor;
        }
        else if (gMaterial.enableLighting == 4)/* SpotLight */
        {
            diffuseFactor = saturate(dot(normal, -spotLightDirectionOnSurface));
            // 入射光の反射ベクトル
            reflectLight = reflect(-spotLightDirectionOnSurface, normal);
              // 内積の計算 
            RdotE = dot(reflectLight, toEye);
            specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
            // Falloff(フォールオフ)を追加する
            float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
            float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (1.0f - gSpotLight.cosAngle));
            diffuseSpotLight = 
            gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * diffuseFactor * gSpotLight.intensity * falloffFactor;
            specularSpotLight = 
            gSpotLight.color.rgb * gSpotLight.intensity * specularPow * falloffFactor;
        }
        
        // 拡散反射
        float3 diffuse = diffuseDirectionalLight + diffusePointLight + diffuseSpotLight;
        // 鏡面反射
        float3 specular = specularDirectionalLight + specularPointLight + specularSpotLight;
        // 拡散反射・鏡面反射
        output.color.rgb = saturate(diffuse + specular);
        // アルファ今まで通り
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    
    return output;
}