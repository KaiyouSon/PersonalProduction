#include "BoundingBox.hlsli"

Texture3D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

// レイからボックスに当たる最近接点と最遠点
float2 ClucRayToBoxCrossPoint(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// レイマーチング
float4 RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// 最小値を0に最大値を1にし値をlerpする
float3 MapValueTo01(float3 boundsMin, float3 boundsMax, float3 value);

// ボックス内にあるかどうかの判定
uint CheckWithInBox(float3 boundsMin, float3 boundsMax, float3 value);

float4 main(V2P i) : SV_TARGET
{
    float3 rayStart = cameraPos;
    float3 rayDir = normalize(i.wpos - cameraPos);
        
    // ボックスの最大最小値
    float3 boundsMin = objectPos + objectScale * float3(-0.5f, -0.5f, -0.5f);
    float3 boundsMax = objectPos + objectScale * float3(+0.5f, +0.5f, +0.5f);

    //float4 resultColor = texColor * hitCount * fogColorRate /*/ stepCount*/;
    float4 resultColor = RayMarching(boundsMin, boundsMax, rayStart, rayDir);
    
    resultColor = resultColor * fogColor * fogColorRate;
    
    //resultColor.a = 1;
    //clip(resultColor.a - 0.1f);
    
    //smoothstep(smoothingClamp.x, smoothingClamp.y, fogColor * hitCount * fogColorRate);
    
    // レイマーチングで球を描画する処理
    //{
    //    float3 v = rayStart - float3(0, 0, 0);
    //    float dot1 = dot(v, rayDir);
    //    float dot2 = dot(v, v) - (0.5f * 0.5f);
    
    //    float discr = (dot1 * dot1) - dot2;
    //    if (dot1 > 0 && dot2 > 0)
    //    {
    //        resultColor = 0;
    //    }
    //    else if (discr < 0)
    //    {
    //        resultColor = 0;qq
    //    }
    //    else
    //    {
    //        resultColor = 1;
    //    }
    //}
    
    return resultColor;
}

// レイからボックスに当たる最近接点と最遠点
float2 ClucRayToBoxCrossPoint(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir)
{
    // 各軸（X、Y、Z）について交点の範囲を計算
    float3 t0 = (boundsMin - rayStart) / rayDir;
    float3 t1 = (boundsMax - rayStart) / rayDir;
    
    // 交点範囲をmin,maxに分ける
    float3 tmin = min(t0, t1);
    float3 tmax = max(t0, t1);
    
    // レイがボックス内に入り、最初に交差するパラメータTを算出（長さ）
    float nearestLength = max(max(tmin.x, tmin.y), tmin.z);
    
    // レイがボックスを通過し、最後に交差するパラメータTを算出（長さ）
    float farthestLength = min(min(tmax.x, tmax.y), tmax.z);
    
    // 最近接点と最遠点のそれぞれの距離を返す
    return float2(max(0, nearestLength), max(0, farthestLength));
}

// レイマーチング
float4 RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir)
{
    float2 hitinfo = ClucRayToBoxCrossPoint(boundsMin, boundsMax, rayStart, rayDir);
    
    // レイの開始点
    float3 rayPos = rayStart;
    
    // 当たった回数を格納する
    uint hitCount = 0;
    
    float colorDensity = 0;
    
    // 光の透過率
    //float transmittance = 1;
    
    for (uint step = 0; step < stepCount; step++)
    {
        if (CheckWithInBox(boundsMin, boundsMax, rayPos) == true)
        {
            float3 uvw = MapValueTo01(boundsMin, boundsMax, rayPos);
            colorDensity += tex.Sample(smp, (uvw + offset)).r * stepLength * density;
        
            //transmittance *= exp(-colorDensity);
        
        }
        
        // 次のレイの座標を算出
        rayPos += rayDir * stepLength;
    }
    
    
    return colorDensity;
    //float4 resultColor = tex.Sample(smp, i.uv);
    
    //return float4(transmittance.xxx, 1 - transmittance);
}

// 最小値を0に最大値を1にし値をlerpする
float3 MapValueTo01(float3 boundsMin, float3 boundsMax, float3 value)
{
    float3 result = (value - boundsMin) / (boundsMax - boundsMin);
    return result;
}

// ボックス内にあるかどうかの判定
uint CheckWithInBox(float3 boundsMin, float3 boundsMax, float3 value)
{
    uint result = (
        value.x >= boundsMin.x && value.x <= boundsMax.x &&
        value.y >= boundsMin.y && value.y <= boundsMax.y &&
        value.z >= boundsMin.z && value.z <= boundsMax.z) ? 1 : 0;
    
    return result;
}