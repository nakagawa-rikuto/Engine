struct PixlShaderOutput
{
    float4 color : SV_TARGET0;
};

PixlShaderOutput main()
{
    PixlShaderOutput output;
    output.color = float4(1.0, 1.0, 1.0, 1.0);
    return output;
}