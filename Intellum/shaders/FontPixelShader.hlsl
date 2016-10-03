Texture2D shaderTexture;
SamplerState SampleType;

cbuffer ColorBuffer
{
    float colorOverloadEnabled;
    float3 padding;
	float4 colorOverload;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 viewDirection : TEXCOORD1;
};

float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.tex);

    if (colorOverloadEnabled == 1.0f)
    {
        //Need to cull all pixels that are not white
        if (textureColor.r >= 0.9 && textureColor.g >= 0.9 && textureColor.b >= 0.9)
        {
            return textureColor;
        }

        return float4(0.0, 0.0, 0.0, 0.0);
    }

    return textureColor;
}