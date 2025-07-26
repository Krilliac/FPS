# add-shaders.ps1
# PowerShell script to create advanced HLSL shader files under Shaders\HLSL in an FPS project.
# Saves files as UTF8 without BOM.

param(
    [string]$ProjectRoot = ".",
    [string]$ProjectName = "FPS"
)

function Write-Header {
    param($Text)
    Write-Host ""
    Write-Host "=== $Text ===" -ForegroundColor Cyan
}

Write-Header "Starting shader creation"
Write-Host "Project Root: $ProjectRoot" -ForegroundColor Yellow
Write-Host "Project: $ProjectName" -ForegroundColor Yellow

# Compute shader directory path
$rootPath  = Resolve-Path $ProjectRoot
$shaderDir = Join-Path -Path $rootPath -ChildPath "$ProjectName\Shaders\HLSL"
if (-not (Test-Path -Path $shaderDir)) {
    New-Item -ItemType Directory -Path $shaderDir -Force | Out-Null
    Write-Host "Created directory: $shaderDir" -ForegroundColor Gray
}

# Define shader files and contents
$shaders = @{
    "PBRSurface.hlsl"        = @"
cbuffer PerFrame : register(b0)
{
    float4x4 World; float4x4 View; float4x4 Projection;
    float3   CameraPos; float    _padding;
};
Texture2D AlbedoMap       : register(t0);
Texture2D NormalMap       : register(t1);
Texture2D MetalRoughMap   : register(t2);
Texture2D OcclusionMap    : register(t3);
SamplerState SamplerLinear: register(s0);
struct VS_IN { float3 Pos:POSITION; float3 Norm:NORMAL; float2 UV:TEXCOORD0; float3 Tangent:TANGENT; };
struct PS_IN { float4 Pos:SV_POSITION; float3 WorldPos:TEXCOORD0; float3 Normal:NORMAL; float2 UV:TEXCOORD1; float3 Tangent:TEXCOORD2; };
PS_IN VS_Main(VS_IN input)
{
    PS_IN o; float4 worldPos = mul(float4(input.Pos,1), World);
    o.WorldPos = worldPos.xyz;
    o.Pos = mul(mul(worldPos, View), Projection);
    o.Normal = normalize(mul(input.Norm, (float3x3)World));
    o.Tangent = normalize(mul(input.Tangent, (float3x3)World));
    o.UV = input.UV; return o;
}
float3 FresnelSchlick(float cosTheta, float3 F0) { return F0 + (1-F0)*pow(1-cosTheta,5); }
float DistributionGGX(float3 N, float3 H, float rough)
{
    float a2=rough*rough; a2*=a2;
    float NdotH=max(dot(N,H),0);
    float denom=NdotH*NdotH*(a2-1)+1;
    return a2/(PI*denom*denom);
}
float GeometrySchlickGGX(float NdotV,float rough)
{
    float k=(rough+1)*(rough+1)/8;
    return NdotV/(NdotV*(1-k)+k);
}
float GeometrySmith(float3 N,float3 V,float3 L,float rough)
{
    return GeometrySchlickGGX(max(dot(N,V),0),rough)*GeometrySchlickGGX(max(dot(N,L),0),rough);
}
float4 PS_Main(PS_IN i):SV_Target
{
    float3 albedo=pow(AlbedoMap.Sample(SamplerLinear,i.UV).rgb,2.2);
    float3 N=normalize(i.Normal);
    float3 V=normalize(CameraPos-i.WorldPos);
    float metal=MetalRoughMap.Sample(SamplerLinear,i.UV).r;
    float rough=MetalRoughMap.Sample(SamplerLinear,i.UV).g;
    float ao=OcclusionMap.Sample(SamplerLinear,i.UV).r;
    float3 L=normalize(float3(-0.5,-1,-0.3));
    float3 H=normalize(V+L);
    float NDF=DistributionGGX(N,H,rough);
    float G=GeometrySmith(N,V,L,rough);
    float3 F=FresnelSchlick(max(dot(H,V),0),lerp(float3(0.04,0.04,0.04),albedo,metal));
    float3 nom=NDF*G*F;
    float denom=4*max(dot(N,V),0)*max(dot(N,L),0)+0.0001;
    float3 spec=nom/denom;
    float3 kS=F;
    float3 kD=(1-kS)*(1-metal);
    float NdotL=max(dot(N,L),0);
    float3 Lo=(kD*albedo/PI+spec)*NdotL;
    float3 ambient=float3(0.03,0.03,0.03)*albedo*ao;
    float3 color=ambient+Lo;
    color=pow(color,1/2.2);
    return float4(color,1);
}
"@
    "EnvReflection.hlsl"       = @"
cbuffer PerFrame : register(b0)
{
    float4x4 WVProj; float3 CameraPos; float Roughness;
};
TextureCube EnvMap   : register(t0);
SamplerState EnvSamp : register(s0);
struct VS_OUT { float4 Pos:SV_POSITION; float3 WorldPos:TEXCOORD0; float3 Normal:TEXCOORD1; };
VS_OUT VS_Env(float3 pos:POSITION,float3 norm:NORMAL)
{
    VS_OUT o; o.WorldPos=pos;
    o.Pos=mul(float4(pos,1),WVProj);
    o.Normal=normalize(norm);
    return o;
}
float4 PS_Env(VS_OUT i):SV_Target
{
    float3 R=reflect(normalize(i.WorldPos-CameraPos),normalize(i.Normal));
    float3 col=EnvMap.SampleLevel(EnvSamp,R,Roughness*64);
    return float4(col,1);
}
"@
    "BloomExtract.hlsl"       = @"
Texture2D InputTex:register(t0);SamplerState Sam:register(s0);
cbuffer ThresholdCB:register(b0){float Threshold;}
float4 PS_BloomExtract(float2 uv:TEXCOORD):SV_Target
{
    float3 c=InputTex.Sample(Sam,uv).rgb;
    float lum=dot(c,float3(0.299,0.587,0.114));
    return float4((lum>Threshold)?c:0,1);
}
"@
    "GaussianBlur.hlsl"       = @"
Texture2D InputTex:register(t0);SamplerState Sam:register(s0);
cbuffer BlurCB:register(b0){float2 TexelSize;}
static const float weights[5]={0.204164,0.304005,0.193783,0.0727,0.0155};
float4 PS_BlurH(float2 uv:TEXCOORD):SV_Target{float4 c=InputTex.Sample(Sam,uv)*weights[0];
    for(int i=1;i<5;i++){c+=InputTex.Sample(Sam,uv+float2(TexelSize.x*i,0))*weights[i];c+=InputTex.Sample(Sam,uv-float2(TexelSize.x*i,0))*weights[i];}
    return c;
}
float4 PS_BlurV(float2 uv:TEXCOORD):SV_Target{float4 c=InputTex.Sample(Sam,uv)*weights[0];
    for(int i=1;i<5;i++){c+=InputTex.Sample(Sam,uv+float2(0,TexelSize.y*i))*weights[i];c+=InputTex.Sample(Sam,uv-float2(0,TexelSize.y*i))*weights[i];}
    return c;
}
"@
    "DebugVisualize.hlsl"     = @"
Texture2D NormalTex:register(t0);
Texture2D DepthTex:register(t1);
SamplerState Sam:register(s0);
struct PS_IN{float2 uv:TEXCOORD;};
float4 PS_DebugNorm(PS_IN i):SV_Target{return float4(NormalTex.Sample(Sam,i.uv).xyz*0.5+0.5,1);}
float4 PS_DebugDepth(PS_IN i):SV_Target{float d=DepthTex.Sample(Sam,i.uv).r;return float4(d,d,d,1);}
"@
    "Utils.hlsl"              = @"
#ifndef UTILS_H
#define UTILS_H
static const float PI=3.14159265359;
static const float HALF_PI=1.57079632679;
float3 GammaToLinear(float3 c){return pow(c,2.2);}
float3 LinearToGamma(float3 c){return pow(c,1/2.2);}
float Remap(float v,float a,float b,float c,float d){return c+(v-a)*(d-c)/(b-a);}
float2 RotateUV(float2 uv,float angle){float ca=cos(angle),sa=sin(angle);float2 c=uv-0.5;return float2(c.x*ca-c.y*sa,c.x*sa+c.y*ca)+0.5;}
#endif
"@
    "Water.hlsl"              = @"
cbuffer PerFrame:register(b0){float4x4 WorldViewProj;float3 CameraPos;float Time;}
Texture2D RefractionMap:register(t0);
Texture2D ReflectionMap:register(t1);
Texture2D NormalMap:register(t2);
SamplerState SamTr:register(s0);
SamplerState SamRe:register(s1);
struct VS_IN{float3 Pos:POSITION;float2 UV:TEXCOORD0;};
struct PS_IN{float4 Pos:SV_POSITION;float2 UV_Re:TEXCOORD0;float2 UV_Ref:TEXCOORD1;float3 ViewDir:TEXCOORD2;};
PS_IN VS_Water(VS_IN i){PS_IN o;float4 worldPos=float4(i.Pos,1);o.Pos=mul(worldPos,WorldViewProj);
 float2 scroll=i.UV+float2(Time*0.05,Time*0.07);
 float2 dn=NormalMap.Sample(SamTr,scroll).rg*2-1;
 o.UV_Re=i.UV+dn*0.02;o.UV_Ref=i.UV-dn*0.02;
 o.ViewDir=normalize(CameraPos-i.Pos);return o;}
float4 PS_Water(PS_IN i):SV_Target{
 float fresnel=pow(1-saturate(dot(i.ViewDir,float3(0,1,0))),3);
 float4 refr=RefractionMap.Sample(SamTr,i.UV_Re);
 float4 refl=ReflectionMap.Sample(SamRe,i.UV_Ref);
 float4 col=lerp(refr,refl,fresnel);
 col.rgb=lerp(col.rgb,float3(0,0.3,0.5),0.2);
 return float4(col.rgb,1);}
"@
    "Phong.hlsl"              = @"
cbuffer PerFrame:register(b0){float4x4 World;float4x4 ViewProj;float3 LightDir;float Padding;float3 LightColor;float SpecPower;}
struct VS_IN{float3 Pos:POSITION;float3 Normal:NORMAL;};
struct PS_IN{float4 Pos:SV_POSITION;float3 WorldN:TEXCOORD0;float3 WorldV:TEXCOORD1;};
PS_IN VS_Phong(VS_IN i){PS_IN o;float4 wp=mul(float4(i.Pos,1),World);o.Pos=mul(wp,ViewProj);o.WorldN=normalize(mul(i.Normal,(float3x3)World));o.WorldV=normalize(-wp.xyz);return o;}
float4 PS_Phong(PS_IN i):SV_Target{
 float NdotL=max(dot(i.WorldN,-LightDir),0);
 float3 diff=NdotL*LightColor;
 float3 H=normalize(-LightDir+i.WorldV);
 float spec=pow(max(dot(i.WorldN,H),0),SpecPower);
 float3 col=diff+spec*LightColor;return float4(col,1);}
"@
    "Rim.hlsl"                = @"
cbuffer PerFrame:register(b0){float4x4 WVP;float3 CamPos;float RimPower;}
struct VS_IN{float3 Pos:POSITION;float3 Normal:NORMAL;};
struct PS_IN{float4 Pos:SV_POSITION;float3 Norm:TEXCOORD0;float3 View:TEXCOORD1;};
PS_IN VS_Rim(VS_IN i){PS_IN o;float4 wp=float4(i.Pos,1);o.Pos=mul(wp,WVP);o.Norm=normalize(i.Normal);o.View=normalize(CamPos-i.Pos);return o;}
float4 PS_Rim(PS_IN i):SV_Target{float rim=pow(1-saturate(dot(i.Norm,i.View)),RimPower);return float4(rim,rim,rim,1);}
"@
    "SSAO.hlsl"               = @"
cbuffer CB:register(b0){float4x4 InvProj;float2 ScreenSize;float Radius;float Bias;}
Texture2D DepthTex:register(t0);SamplerState Sam:register(s0);
static const float2 samples[16]={float2(1,0),float2(-1,0),float2(0,1),float2(0,-1),float2(.707,.707),float2(-.707,.707),float2(.707,-.707),float2(-.707,-.707),float2(1,1),float2(-1,1),float2(1,-1),float2(-1,-1),float2(.5,0),float2(-.5,0),float2(0,.5),float2(0,-.5)};
float4 main(float2 uv:TEXCOORD):SV_Target{
 float z=DepthTex.Sample(Sam,uv).r;
 float3 pos=mul(float4(uv*2-1,z,1),InvProj).xyz;
 float oc=0;
 for(int i=0;i<16;i++){
  float2 off=samples[i]/ScreenSize*Radius;
  float z2=DepthTex.Sample(Sam,uv+off).r;
  float3 pos2=mul(float4((uv+off)*2-1,z2,1),InvProj).xyz;
  oc+=((pos.z-pos2.z)>Bias)?1:0;
 }
 oc/=16;
 return float4(1-oc,1-oc,1-oc,1);}
"@
    "DebugUtils.hlsl"         = @"
Texture2D DebugTex:register(t0);SamplerState Sam:register(s0);
struct PS_IN{float2 uv:TEXCOORD;};
float4 PS_Checker(PS_IN i):SV_Target{float s=step(0.5,frac(i.uv.x*10))^step(0.5,frac(i.uv.y*10));return float4(s,s,s,1);}
float4 PS_Gradient(PS_IN i):SV_Target{return float4(i.uv.x,i.uv.y,0,1);}
"@
}

# Write shader files
Write-Header "Writing shaders"
foreach ($file in $shaders.Keys) {
    $full = Join-Path -Path $shaderDir -ChildPath $file
    $utf8NoBom = New-Object System.Text.UTF8Encoding $false
    [System.IO.File]::WriteAllText($full, $shaders[$file], $utf8NoBom)
    Write-Host "Created: $file" -ForegroundColor Green
}

# Clean existing .hlsl of BOM
Write-Header "Cleaning BOM"
Get-ChildItem -Path $shaderDir -Filter "*.hlsl" | ForEach-Object {
    $txt       = Get-Content -Raw $_.FullName
    $utf8NoBom = New-Object System.Text.UTF8Encoding $false
    [System.IO.File]::WriteAllText($_.FullName, $txt, $utf8NoBom)
    Write-Host "Cleaned: $($_.Name)" -ForegroundColor Yellow
}

Write-Header "Done"
Write-Host "Shaders added under '$shaderDir'." -ForegroundColor Green```

This script now includes all previously defined shaders plus **Water**, **Phong**, **Rim**, **SSAO**, and **DebugUtils**. It ensures each `.hlsl` is written UTF-8 without BOM and cleans any existing BOM.