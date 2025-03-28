// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewMat;
    matrix worldMat;
    float3 cameraPos;
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

struct SkinOutput
{
    float4 pos;
    float3 normal;
};

struct Appdata
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 boneIndices : BONEINDICES; // ボーンの番号
    float4 boneWeights : BONEWEIGHTS; // ボーンのスキンウェイト
};

struct V2P
{
    float4 svpos : SV_POSITION;
    float4 wpos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};