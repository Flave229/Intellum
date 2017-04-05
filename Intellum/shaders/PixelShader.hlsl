Texture2D shaderTexture[10];
Texture2D lightMap;
Texture2D bumpMap;
SamplerState SampleType;

cbuffer TextureBuffer
{
    float textureCount;
    float lightMapEnabled;
    float bumpMapEnabled;
    float padding2;
};

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

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
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 DefaultPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor = float4(0, 0, 0, 0);

    if (textureCount > 0)
        textureColor = shaderTexture[0].Sample(SampleType, input.tex);

    for (int i = 1; i < textureCount; i++)
        textureColor *= shaderTexture[i].Sample(SampleType, input.tex) * 2.0f;

    if (lightMapEnabled == 1.0f)
        textureColor *= lightMap.Sample(SampleType, input.tex);

    float3 bumpNormal = float3(0.0f, 0.0f, 0.0f);

    if (bumpMapEnabled == 1.0f)
    {
        float4 bumpMapTexture = bumpMap.Sample(SampleType, input.tex);
        bumpMapTexture = (bumpMapTexture * 2.0f) - 1.0f;

        bumpNormal = (bumpMapTexture.x * input.tangent) + (bumpMapTexture.y * input.binormal) + (bumpMapTexture.z * input.normal);
        bumpNormal = normalize(bumpNormal);
    }

	float3 lightDir = -lightDirection;
    float lightIntensity = 0.0f;

    if (bumpMapEnabled == 1.0f)
        lightIntensity = saturate(dot(bumpNormal, lightDir));
    else
        lightIntensity = saturate(dot(input.normal, lightDir));

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
    
    if (textureCount > 0)
        color *= textureColor;

    if (colorOverloadEnabled)
        color *= colorOverload;

	color = saturate(color + specular);

	return color;
}