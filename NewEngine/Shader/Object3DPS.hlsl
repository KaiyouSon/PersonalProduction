#include "Object3D.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutputSvposPosNormalUv vsOutput) : SV_TARGET
{
	// テクスチャーマッピング
    float4 texColor = tex.Sample(smp, vsOutput.uv);
    
	// 光沢度
    const float shininess = 4.0f;

	// 頂点から視点へのベクトル
    float3 eyeDir = normalize(cameraPos - vsOutput.worldPos.xyz);

    // マテリアル
    Material material = { ambient, diffuse, specular };
    
    // シェーダーカラー
    float4 shaderColor = float4(float3(1, 1, 1) * material.ambient, alpha);
    
	// 平行光源
    for (int i = 0; i < directionalLightNum; i++)
    {
        float3 color = CalculateDirectionalLight(
            directionalLights[i], material, vsOutput.normal, eyeDir, shininess);
        shaderColor.rgb += color;
    }

	// 点光源
    for (int i = 0; i < pointLightNum; i++)
    {
        float3 color = CalculatePointLight(
            pointLights[i], material, vsOutput.worldPos.xyz, vsOutput.normal, eyeDir, shininess);
        shaderColor.rgb += color;
    }

	// スポットライト
    for (int i = 0; i < spotLightNum; i++)
    {
        float3 color = CalculateSpotLight(
            spotLights[i], material, vsOutput.worldPos.xyz, vsOutput.normal, eyeDir, shininess);
        shaderColor.rgb += color;
    }

	// 丸影
    for (int i = 0; i < circleShadowNum; i++)
    {
        float3 color = CalculateCircleShadow(circleShadows[i], vsOutput.worldPos.xyz);
        shaderColor.rgb += color;
    }

    if (isActiveFog == true)
    {
		// カメラの座標と頂点の距離
        float dis = distance(vsOutput.worldPos.xyz, cameraPos);
        float rate = smoothstep(fogNearDis, fogFarDis, dis);

		// フォグの色
        float4 tFogColor = fogColor * rate;
        float4 outPutColor = shaderColor * texColor * color;
        return fogColor * rate + outPutColor * (1 - rate);
    }
    else
    {
        return shaderColor * texColor * color;
    }
}