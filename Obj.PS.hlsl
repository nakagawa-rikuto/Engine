struct PixlShaderOutput
{
    float4 color : SV_TARGET0;
};

struct Material
{
    float4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

PixlShaderOutput main()
{
    PixlShaderOutput output;
    output.color = gMaterial.color;
    return output;
}