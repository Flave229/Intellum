Texture2D shaderTexture[10];
Texture2D lightMap;
Texture2D bumpMap;
SamplerState SampleType;

cbuffer TextureBuffer : register(b2)
{
    float textureCount;
    float lightMapEnabled;
    float bumpMapEnabled;
    float padding2;
};

cbuffer LightBuffer : register(b3)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

cbuffer ColorBuffer : register(b4)
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

float4 CalculateTextureColor(float2 inputTextureCordinates)
{
    float4 textureColor = float4(0, 0, 0, 0);

    if (textureCount <= 0)
        return textureColor;
    
    textureColor = shaderTexture[0].Sample(SampleType, inputTextureCordinates);

    for (int i = 1; i < textureCount; i++)
        textureColor *= shaderTexture[i].Sample(SampleType, inputTextureCordinates) * 2.0f;

    if (lightMapEnabled == 1.0f)
        textureColor *= lightMap.Sample(SampleType, inputTextureCordinates);

    return textureColor;
}

float3 CalculateBumpNormal(float2 inputTextureCordinates, float3 tangent, float3 binormal, float3 normal)
{
    float3 bumpNormal = float3(0.0f, 0.0f, 0.0f);

    if (bumpMapEnabled == 0.0f)
        return bumpNormal;

    float4 bumpMapTexture = bumpMap.Sample(SampleType, inputTextureCordinates);
    bumpMapTexture = (bumpMapTexture * 2.0f) - 1.0f;

    bumpNormal = (bumpMapTexture.x * tangent) + (bumpMapTexture.y * binormal) + (bumpMapTexture.z * normal);
    return normalize(bumpNormal);
}

float4 CalculateSpecularLight(float3 normal, float3 viewDirection, float3 lightDirection, float lightIntensity)
{
    float3 reflection = normalize(2 * lightIntensity * normal - lightDirection);

    return pow(saturate(dot(reflection, viewDirection)), specularPower);
}

float4 DefaultPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor = CalculateTextureColor(input.tex);

    float3 bumpNormal = CalculateBumpNormal(input.tex, input.tangent, input.binormal, input.normal);

	float3 lightDir = -lightDirection;
    float lightIntensity = 0.0f;

    if (bumpMapEnabled == 1.0f)
        lightIntensity = saturate(dot(bumpNormal, lightDir));
    else
        lightIntensity = saturate(dot(input.normal, lightDir));

	float4 color = ambientColor;

	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if (lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity);
		color = saturate(color);

        specular = CalculateSpecularLight(input.normal, input.viewDirection, lightDir, lightIntensity);
    }
    
    if (textureCount > 0)
        color *= textureColor;

    if (colorOverloadEnabled)
        color *= colorOverload;

	color = saturate(color + specular);

	return color;
}