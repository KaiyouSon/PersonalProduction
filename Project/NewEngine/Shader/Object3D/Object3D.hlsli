// マテリアル
struct Material
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

// 平行光源
struct DirectionalLight
{
    float4 color;
    float3 vec;
    uint isActive;
};

// 点光源
struct PointLight
{
    float4 color;
    float length;
    float3 pos;
    float3 atten;
    uint isActive;
};

// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewProjMat;
    matrix lightViewProjMat;
    matrix worldMat;
    float3 cameraPos;
    float3 lightCameraPos;
}

// マテリアル
cbuffer ConstantBufferDataMaterial : register(b1)
{
    float3 ambient : packoffset(c0); // アンビエント係数
    float3 diffuse : packoffset(c1); // ディフューズ係数
    float3 specular : packoffset(c2); // スペキュラー係数
    float alpha : packoffset(c2.w); // アルファ
}

// 色
cbuffer ConstantBufferDataColor : register(b2)
{
    float4 color; // 色
}

static const int maxBones = 32;
cbuffer ConstantBufferDataSkinning : register(b3)
{
    matrix skinningMat[maxBones];
}

// --- フォグ ---------------------- //
cbuffer ConstantBufferDataUVParameter : register(b4)
{
    float2 offset;
    float2 tiling;
};

// ライトグループ
static const uint directionalLightSize = 1;
static const uint pointLightSize = 10;
cbuffer ConstantBufferDataLightGroup : register(b5)
{
    DirectionalLight directionalLight[directionalLightSize];
    PointLight pointLight[pointLightSize];
};

cbuffer ConstantBufferDissolve : register(b6)
{
    float dissolve;
    float colorPower;
    float4 dissolveColor;
}

cbuffer ConstantBufferShadow : register(b7)
{
    uint isWriteShadow;
}

struct Appdata
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 wpos : POSITION0; // ワールド座標
    float3 normal : NORMAL; // 法線ベクトル
    float2 uv : TEXCOORD; // uv値
    float4 spos : POSITIONT1;
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};
