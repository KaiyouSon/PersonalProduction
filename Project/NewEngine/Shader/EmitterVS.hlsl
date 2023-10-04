#include "Emitter.hlsli"

struct OutputData
{
    float3 curPos;
    float2 curScale;
    float curRot;
    float curShininess;
    float4 curColor;
};

StructuredBuffer<OutputData> inputData : register(t1);

V2G main(Appdata i)
{
    V2G o = (V2G) 0;
    o.pos = float4(inputData[0].curPos, 1);
    o.scale = i.scale;
    o.rot = i.rot;
    o.shininess = i.shininess;
    o.color = i.color;
    return o;
}