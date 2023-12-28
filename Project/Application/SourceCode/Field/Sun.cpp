#include "Sun.h"

Sun::Sun() :
	mSun(std::make_unique<ParticleObject>())
{
	mSun->SetParticleTexture(TextureManager::GetTexture("Particle2"));
	mType = FieldObjectType::Sun;
}

void Sun::Init()
{
	mSun->SetParticleData<ParticleParameter::PonDeRing>(ModelManager::GetModel("PonDeRing"), 100);
	mSun->SetComputePipeline(PipelineManager::GetComputePipeline("PonDeRingInit"));
	mSun->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("PonDeRing"));
	mSun->ExecuteCS(4160);
	mSun->SetComputePipeline(PipelineManager::GetComputePipeline("PonDeRingUpdate"));
}

void Sun::Update()
{
	mSun->Update();
}

void Sun::ExecuteCS()
{
	mSun->ExecuteCS(1625);
}

void Sun::Draw(const bool isDrawDepth)
{
	isDrawDepth;
	mSun->Draw("Object3D");
}

void Sun::SetTransform(const Transform& transform)
{
	mSun->pos = transform.pos;
	mSun->scale = transform.scale;
	mSun->rot = transform.rot;
}

Vec3 Sun::GetPos()
{
	return mSun->pos;
}
