cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 WorldPos : POSITION1;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    
    float4 worldPos = mul(input.Pos, World);
    output.WorldPos = worldPos.xyz;
    output.Pos = mul(mul(worldPos, View), Projection);
    output.Normal = normalize(mul(input.Normal, (float3x3)World));
    output.Tex = input.Tex;
    
    return output;
}