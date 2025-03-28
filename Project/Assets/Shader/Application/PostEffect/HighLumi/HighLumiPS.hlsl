#include "HighLumi.hlsli"

Texture2D<float4> tex1 : register(t0);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex1.Sample(smp, i.uv);
    
    float grayScale = texColor.r * 0.299f + texColor.g * 0.587f + texColor.b * 0.114f;
    float extract = smoothstep(smoothClamp.x, smoothClamp.y, texColor.r);
    
    //float4 result = float4(texColor.rgb * extract, 1);
    float4 result = float4(texColor.rgb, 1);
    return result;
}
