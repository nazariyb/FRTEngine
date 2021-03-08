struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput VShader(float4 position : POSITION, float4 color : COLOR)
{
    PSInput result;

    result.position = position;
    result.color = color;

    return result;
}

float4 PShader(PSInput input) : SV_TARGET
{
    return input.color;
    //return float4(0.9f, 0.9f, 0.9f, 1.0f);
}