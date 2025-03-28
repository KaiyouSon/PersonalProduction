#include "BossAttackTrajectoryEffect.h"
using namespace StructuredBufferData;
using namespace ConstantBufferData;
using namespace ParticleParameter;

BossAttackTrajectoryEffect::BossAttackTrajectoryEffect() :
	mEmitter(std::make_unique<GPUEmitter>())
{
	mEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mEmitter->SetParticleData<BossAttackTrajectoryParticle>(10240);
	mEmitter->AddCSConstantBuffer<CBossAttackTrajectoryEffect>();
	mEmitter->AddCSStructuredBuffer<STimer>();

	mEffectType = EffectType::BossAttackTrajectoryEffect;
}

void BossAttackTrajectoryEffect::Generate()
{
	mIsActive = true;
	mIsGenerate = false;

	mEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("BossAttackTrajectoryEffect"));
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("BossAttackTrajectoryEffectInit"));

	// ベクトル転送
	CBossAttackTrajectoryEffect data = { mIsGenerate,mStartPos,mEndPos };
	mEmitter->TransferCSConstantBuffer(1, data);

	mEmitter->ExecuteCS();
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("BossAttackTrajectoryEffectUpdate"));
}

void BossAttackTrajectoryEffect::Update()
{
	// ベクトル転送
	CBossAttackTrajectoryEffect data = { mIsGenerate,mStartPos,mEndPos };
	mEmitter->TransferCSConstantBuffer(1, data);

	mEmitter->Update();
}

void BossAttackTrajectoryEffect::ExecuteCS()
{
	mEmitter->ExecuteCS();
}

void BossAttackTrajectoryEffect::Draw()
{
	mEmitter->Draw("Object3D");
}

void BossAttackTrajectoryEffect::Execute(const bool isGenerate, const Vec3 startPos, const Vec3 endPos)
{
	mIsGenerate = isGenerate;
	mStartPos = startPos;
	mEndPos = endPos;
}
