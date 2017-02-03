Texture2D shaderTexture[2];
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 DefaultPixelShader(PixelInputType input) : SV_TARGET
{
    float4 texture1Color = shaderTexture[0].Sample(SampleType, input.tex);
    float4 texture2Color = shaderTexture[1].Sample(SampleType, input.tex);

    float4 textureColor = texture1Color * texture2Color * 2.0;

	float3 lightDir = -lightDirection;
	float lightIntensity = saturate(dot(input.normal, lightDir));
	float4 color = ambientColor;
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 reflection;
	
	if (lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity);
		color = saturate(color);

		reflection = normalize(2 * lightIntensity * input.normal - lightDir);

		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}

    color = color * textureColor;
	color = saturate(color + specular);

	return color;
}