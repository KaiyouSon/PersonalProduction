// 3D変換行列
cbuffer ConstBufferDataTransform : register(b0)
{
    matrix mat; // 3D変換行列
}

// 色
cbuffer ConstBufferDataMaterial : register(b1)
{
    float4 color; // 色(RGBA)
};

cbuffer ConstBufferDataVignette : register(b2)
{
    float4 vignetteColor; // 色(RGBA)
    float2 vignetteRange;
};

struct AppData
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};
