#include "ParticleMesh.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

void ParticleMesh::Update()
{
	if (!isActive)
	{
		return;
	}

	BaseUpdate();

	if (mTextureComponent->GetTexture()->GetTag() != "White")
	{
		mMeshTexture = mTextureComponent->GetTexture();
	}

	// マテリアルの転送
	MaterialTransfer();
}

void ParticleMesh::ExecuteCS()
{
	if (!isActive)
	{
		return;
	}

	if (mMeshTexture == nullptr)
	{
		return;
	}

	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	CSMaterialTransfer();

	mComputePipeline->DrawCommand();

	CSMaterialDrawCommands();

	// ディスパッチ
	cmdList->Dispatch(1, 1, 1);
}

void ParticleMesh::AppedToRenderer()
{
	if (!isActive)
	{
		return;
	}

	gCurrentScene->GetRenderer()->Register(layerTag,
		[this]()
		{
			DrawCommands();
		});
}

void ParticleMesh::Draw(const std::string& _layerTag, const BlendMode _blendMode)
{
	_layerTag;
	_blendMode;

	if (!mParticleTexture) return;
	if (!mParticleDataSB->GetBufferResource()) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	MaterialDrawCommands();

	// SRVの設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(startIndex, mParticleTexture->GetBufferResource()->srvHandle.gpu);

	if (mParticleDataSB->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		// GENERIC_READ -> UNORDERED_ACCESS に変更
		renderBase->TransitionBufferState(
			mParticleDataSB->GetBufferResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	// CSの結果
	cmdList->SetGraphicsRootDescriptorTable(
		startIndex + 1, mParticleDataSB->GetBufferResource()->srvHandle.gpu);

	cmdList->DrawInstanced(mMaxParticle, 1, 0, 0);
}

void ParticleMesh::SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize)
{
	ITexture* tex = gAssetsManager->GetTexture(textureTag);
	if (!tex)
	{
		return;
	}

	mTextureComponent->SetTexture(textureTag);
	mMeshTexture = mTextureComponent->GetTexture();
}



ParticleMesh::ParticleMesh() :
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ParticleMesh")),
	mComputePipeline(PipelineManager::GetComputePipeline("ParticleMesh")),
	mMeshTexture(TextureManager::GetTexture("White")),
	mParticleTexture(TextureManager::GetTexture("White")),
	mParticleDataSB(std::make_unique<StructuredBuffer>()),
	mCSMaterial(std::make_unique<Material>())
{
	InitComponents();

	// マテリアルの初期化
	MaterialInit();
	CSMaterialInit();

	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);
}

ParticleMesh::ParticleMesh(const std::string& name) :
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ParticleMesh")),
	mComputePipeline(PipelineManager::GetComputePipeline("ParticleMesh")),
	mMeshTexture(TextureManager::GetTexture("White")),
	mParticleTexture(TextureManager::GetTexture("Particle1")),
	mParticleDataSB(std::make_unique<StructuredBuffer>()),
	mCSMaterial(std::make_unique<Material>())
{
	this->name = name;

	InitComponents();

	// マテリアルの初期化
	MaterialInit();
	CSMaterialInit();

	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);

	SetParticleData<ParticleParameter::Default>(10000);
}

void ParticleMesh::InitComponents()
{
	layerTag = "Object3D";
	mType = GameObjectType::ParticleMesh;

	mComponentManager->AddComponent<ParticleMeshInfo>();
	mComponentManager->AddComponent<Transform>();
	mComponentManager->AddComponent<TextureComponent>();

	mTransform = mComponentManager->GetComponent<Transform>();
	mTextureComponent = mComponentManager->GetComponent<TextureComponent>();
}

void ParticleMesh::DrawCommands()
{
	if (!mParticleTexture) return;
	if (!mParticleDataSB->GetBufferResource()) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	MaterialDrawCommands();

	// SRVの設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(startIndex, mParticleTexture->GetBufferResource()->srvHandle.gpu);

	if (mParticleDataSB->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		// GENERIC_READ -> UNORDERED_ACCESS に変更
		renderBase->TransitionBufferState(
			mParticleDataSB->GetBufferResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	// CSの結果
	cmdList->SetGraphicsRootDescriptorTable(
		startIndex + 1, mParticleDataSB->GetBufferResource()->srvHandle.gpu);

	cmdList->DrawInstanced(mMaxParticle, 1, 0, 0);
}
// --- マテリアル関連 --------------------------------------------------- //
void ParticleMesh::MaterialInit()
{
	mMaterial = std::make_unique<Material>();
	//mMaterial->Copy(*gAssetsManager->GetMaterial("BasicParticleMesh"));

	if (mMaterial->constantBuffers.empty())
	{
		mMaterial->AddConstantBuffer<CTransformP>();	// 2D行列
		mMaterial->AddConstantBuffer<CColor>();			// 色
		mMaterial->AddConstantBuffer<CUVParameter>();	// UVパラメータ
	}
	else
	{
		mMaterial->SetConstantBuffer<CTransform2D>(0);	// 2D行列
		mMaterial->SetConstantBuffer<CColor>(1);		// 色
		mMaterial->SetConstantBuffer<CUVParameter>(2);	// UVパラメータ
	}
	mMaterial->Init();
}
void ParticleMesh::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	if (!mMaterial)
	{
		return;
	}

	// トランスフォーム
	CTransformP transformPData =
	{
		Camera::current.GetViewMat() * Camera::current.GetPerspectiveMat(),
		mTransform->GetWorldMat(),
		mBillboard.GetMat(),
	};
	mMaterial->TransferDataToConstantBuffer(0, transformPData);

	// 色データ
	CColor colorData = { color.To01() };
	mMaterial->TransferDataToConstantBuffer(1, colorData);

	// UVパラメーター
	CUVParameter uvData = { offset,tiling };
	mMaterial->TransferDataToConstantBuffer(2, uvData);
}
void ParticleMesh::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

void ParticleMesh::CSMaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// テクスチャーの面積
	iConstantBuffer = std::make_unique<ConstantBuffer<CTextureSizeData>>();
	mCSMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	mCSMaterial->Init();
}
void ParticleMesh::CSMaterialTransfer()
{
	// メッシュテクスチャ情報
	CTextureSizeData textureAreaSize =
	{
		Vec2(mMeshTexture->GetInitalSize().x, mMeshTexture->GetInitalSize().y),
		static_cast<float>(mMaxParticle)
	};
	TransferDataToConstantBuffer(mCSMaterial->constantBuffers[0].get(), textureAreaSize);
}
void ParticleMesh::CSMaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = RenderBase::GetInstance()->GetCommandList();

	// ディスクリプターヒープ設定
	auto srvDescHeap = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	cmdList->SetDescriptorHeaps(1, &srvDescHeap);

	// CBV
	uint32_t cbvStartIndex = mComputePipeline->GetRootSignature()->GetCBVStartIndex();
	for (uint32_t i = 0; i < mCSMaterial->constantBuffers.size(); i++)
	{
		cmdList->SetComputeRootConstantBufferView(
			cbvStartIndex + i, mCSMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}

	// SRV
	uint32_t srvStartIndex = mComputePipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetComputeRootDescriptorTable(srvStartIndex, mMeshTexture->GetBufferResource()->srvHandle.gpu);

	if (mParticleDataSB->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_GENERIC_READ)
	{
		// GENERIC_READ -> UNORDERED_ACCESS に変更
		renderBase->TransitionBufferState(
			mParticleDataSB->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}

	// UAV
	uint32_t uavStartIndex = mComputePipeline->GetRootSignature()->GetUAVStartIndex();
	cmdList->SetComputeRootDescriptorTable(uavStartIndex, mParticleDataSB->GetBufferResource()->uavHandle.gpu);
	// その他のUAVデータ
	for (uint32_t i = 0; i < mStructuredBuffers.size(); i++)
	{
		cmdList->SetComputeRootDescriptorTable(uavStartIndex + i + 1, mStructuredBuffers[i]->GetBufferResource()->uavHandle.gpu);
	}
}



// --- セッター -------------------------------------------------------- //ko

// メッシュのテクスチャー	
void ParticleMesh::SetMeshTexture(Texture* meshTexture)
{
	mMeshTexture = meshTexture;
	mMaxParticle = static_cast<uint32_t>((mMeshTexture->GetInitalSize().x * mMeshTexture->GetInitalSize().y));
}

// パーティクルのテクスチャー
void ParticleMesh::SetParticleTexture(Texture* particleTexture) { mParticleTexture = particleTexture; }

// Graphicsパイプラインを設定
void ParticleMesh::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// Computeパイプラインを設定
void ParticleMesh::SetComputePipeline(ComputePipeline* computePipeline) { mComputePipeline = computePipeline; }

// --- ゲッター -------------------------------------------------------- //

Vec3 ParticleMesh::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform->GetWorldMat(), true);
	return worldPos;
}

Vec3 ParticleMesh::GetWorldScale()
{
	Vec3 worldScale = mTransform->GetWorldMat().GetScale();
	return worldScale;
}
