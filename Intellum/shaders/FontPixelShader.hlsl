Texture2D shaderTexture[10];
Texture2D lightMap;
SamplerState SampleType;

cbuffer ColorBuffer : register(b2)
{
    float colorOverloadEnabled;
    float3 padding;
	float4 colorOverload;
};

cbuffer TextureBuffer : register(b3)
{
    float textureCount;
    float lightMapEnabled;
    float2 padding2;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 viewDirection : TEXCOORD1;
};

float4 CalculateTextureColor(float2 inputTextureCordinates)
{
    float4 textureColor = float4(0, 0, 0, 0);

    if (textureCount <= 0)
        return textureColor;

    textureColor = shaderTexture[0].Sample(SampleType, inputTextureCordinates);

    for (int i = 1; i < textureCount; i++)
    {
        textureColor *= shaderTexture[i].Sample(SampleType, inputTextureCordinates) * 2.0f;
    }

    if (lightMapEnabled == 1.0f)
        textureColor *= lightMap.Sample(SampleType, inputTextureCordinates);

    return textureColor;
}

float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
    float4 color = CalculateTextureColor(input.tex);
    
    if (colorOverloadEnabled == 1.0f)
    {
        //Need to cull all pixels that are not white
        if (color.r >= 0.9 && color.g >= 0.9 && color.b >= 0.9)
        {
            return colorOverload;
        }
        else
        {
            clip(-1.0f);
        }

        return float4(0.0, 0.0, 0.0, 0.0);
    }

    return color;
}