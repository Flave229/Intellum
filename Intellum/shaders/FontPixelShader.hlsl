Texture2D shaderTexture;
SamplerState SampleType;

cbuffer ColorBuffer
{
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

    return textureColor;
}