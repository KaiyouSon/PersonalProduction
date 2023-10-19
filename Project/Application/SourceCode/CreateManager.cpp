#include "CreateManager.h"

// シェダーコンパイラーの生成
void CreateManager::CreateShaderCompiler()
{
	std::string path1 = "NewEngine/Shader/";
	std::string path2 = "Application/Shader/";

	ShaderCompilerSetting setting;

	// 草用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, 1);
	setting.vsFilePath = path2 + "Grass/GrassVS.hlsl";
	setting.gsFilePath = path2 + "Grass/GrassGS.hlsl";
	setting.psFilePath = path2 + "Grass/GrassPS.hlsl";
	ShaderCompilerManager::Create(setting, "Grass");

	// 雲用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Cloud/CloudVS.hlsl";
	setting.psFilePath = path2 + "Cloud/CloudPS.hlsl";
	ShaderCompilerManager::Create(setting, "Cloud");

	// ビネット用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Vignette/VignetteVS.hlsl";
	setting.psFilePath = path2 + "Vignette/VignettePS.hlsl";
	ShaderCompilerManager::Create(setting, "Vignette");

	// 木の枝用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Branch/BranchVS.hlsl";
	setting.psFilePath = path2 + "Branch/BranchPS.hlsl";
	ShaderCompilerManager::Create(setting, "Branch");

	// リスポーン地点用（下の波紋用）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Ripple/RippleVS.hlsl";
	setting.psFilePath = path2 + "Ripple/RipplePS.hlsl";
	ShaderCompilerManager::Create(setting, "Ripple");

	// リスポーン地点用（浮いてる菱形用）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Rhombus/RhombusVS.hlsl";
	setting.psFilePath = path2 + "Rhombus/RhombusPS.hlsl";
	ShaderCompilerManager::Create(setting, "Rhombus");

	// 高輝度箇所抽出用（RenderTexture）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "HighLumi/HighLumiVS.hlsl";
	setting.psFilePath = path2 + "HighLumi/HighLumiPS.hlsl";
	ShaderCompilerManager::Create(setting, "HighLumi");

	// ガウシアンブラー用（RenderTexture）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "GaussianBlur/GaussianBlurVS.hlsl";
	setting.psFilePath = path2 + "GaussianBlur/GaussianBlurPS.hlsl";
	ShaderCompilerManager::Create(setting, "GaussianBlur");

	// 合成用（RenderTexture）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Composite/CompositeVS.hlsl";
	setting.psFilePath = path2 + "Composite/CompositePS.hlsl";
	ShaderCompilerManager::Create(setting, "Composite");

	// リスポーン時の遷移用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "RespawnTransition/RespawnTransitionVS.hlsl";
	setting.psFilePath = path2 + "RespawnTransition/RespawnTransitionPS.hlsl";
	ShaderCompilerManager::Create(setting, "RespawnTransition");

	// ShadowObj用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "ShadowObj/ShadowObjVS.hlsl";
	setting.psFilePath = path2 + "ShadowObj/ShadowObjPS.hlsl";
	ShaderCompilerManager::Create(setting, "ShadowObj");

	// ShadowMap用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "ShadowMap/ShadowMapVS.hlsl";
	setting.psFilePath = path2 + "ShadowMap/ShadowMapPS.hlsl";
	ShaderCompilerManager::Create(setting, "ShadowMap");

	// リスポーンエフェクト用
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "RespawnPointEffect/RespawnPointEffectCS.hlsl";
	setting.vsFilePath = path2 + "RespawnPointEffect/RespawnPointEffectVS.hlsl";
	setting.gsFilePath = path1 + "EmitterGS.hlsl";
	setting.psFilePath = path1 + "EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "RespawnPointEffect");

	// ボス攻撃モーション1初期化用
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "BossAttack1Effect/BossAttack1EffectInitCS.hlsl";
	setting.vsFilePath = path2 + "BossAttack1Effect/BossAttack1EffectVS.hlsl";
	setting.gsFilePath = path1 + "ParticleMeshGS.hlsl";
	setting.psFilePath = path1 + "ParticleMeshPS.hlsl";
	ShaderCompilerManager::Create(setting, "BossAttack1EffectInit");

	// ボス攻撃モーション1初期化用
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "BossAttack1Effect/BossAttack1EffectUpdateCS.hlsl";
	setting.vsFilePath = path2 + "BossAttack1Effect/BossAttack1EffectVS.hlsl";
	setting.gsFilePath = path1 + "ParticleMeshGS.hlsl";
	setting.psFilePath = path1 + "ParticleMeshPS.hlsl";
	ShaderCompilerManager::Create(setting, "BossAttack1EffectUpdate");

	// ボリューメトリックフォグ用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.vsFilePath = path2 + "VolumetricFog/VolumetricFogVS.hlsl";
	setting.psFilePath = path2 + "VolumetricFog/VolumetricFogPS.hlsl";
	ShaderCompilerManager::Create(setting, "VolumetricFog");
}

// パイプライン生成
void CreateManager::CreateGraphicsPipeline()
{
	GraphicsPipelineSetting setting;
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};

	// 草用
	setting = PipelineManager::GetGraphicsPipeline("Emitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Grass");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Grass");

	// 雲用
	depthStencilDesc = D3D12_DEPTH_STENCIL_DESC();
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Cloud");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Cloud");

	// 天球用（RenderTexture）
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Vignette");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "Vignette");

	// 木の枝用
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Branch");
	setting.cullMode = CullMode::None;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Branch");

	// リスポーン地点用（下の波紋用）
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Ripple");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Ripple");

	// リスポーン地点用（浮いてる菱形用）
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Rhombus");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleList;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Rhombus");

	// 高輝度箇所抽出用（RenderTexture）
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("HighLumi");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "HighLumi");

	// ガウシアンブラー用（RenderTexture）
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("GaussianBlur");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "GaussianBlur");

	// 合成用（RenderTexture）
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Composite");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Composite");

	// リスポーン時の遷移用
	setting = PipelineManager::GetGraphicsPipeline("Sprite")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("RespawnTransition");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 4;
	PipelineManager::CreateGraphicsPipeline(setting, "RespawnTransition");

	// ShadowObj用
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ShadowObj");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowObj");

	// ShadowMap用
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ShadowMap");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowMap");

	// 深度値のみ書き込み用
	{
		// 3Dオブジェクト
		setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "Object3DWriteNone");

		// 草
		setting = PipelineManager::GetGraphicsPipeline("Grass")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "GrassWriteNone");
	}

	// リスポーンエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("RespawnPointEffect");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "RespawnPointEffect");

	// ボス攻撃モーション1用
	setting = PipelineManager::GetGraphicsPipeline("ParticleMesh")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("BossAttack1EffectInit");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "BossAttack1Effect");

	// ボリューメトリックフォグ用
	depthStencilDesc = D3D12_DEPTH_STENCIL_DESC();
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	setting = GraphicsPipelineSetting();
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("VolumetricFog");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 5;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "VolumetricFog");
}

// Computeパイプラインの生成
void CreateManager::CreateComputePipeline()
{
	// リスポーンエフェクト用
	ComputePipelineSetting setting;
	setting = ComputePipelineSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("RespawnPointEffect");
	setting.rootSignatureSetting.maxCbvRootParameter = 0;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "RespawnPointEffect");

	// リスポーンエフェクト用
	setting = PipelineManager::GetComputePipeline("ParticleMesh")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("BossAttack1EffectInit");
	PipelineManager::CreateComputePipeline(setting, "BossAttack1EffectInit");

	// リスポーンエフェクト用
	setting = PipelineManager::GetComputePipeline("ParticleMesh")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("BossAttack1EffectUpdate");
	PipelineManager::CreateComputePipeline(setting, "BossAttack1EffectUpdate");
}

void CreateManager::Create()
{
	// シェダーコンパイラーの生成
	CreateShaderCompiler();

	// Graphicsパイプライン生成
	CreateGraphicsPipeline();

	// Computeパイプライン生成
	CreateComputePipeline();
}
