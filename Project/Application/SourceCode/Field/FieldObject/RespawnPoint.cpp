#include "RespawnPoint.h"
#include "EffectManager.h"

RespawnPoint::RespawnPoint() :
	mRipple(std::make_unique<Object3D>()),
	mRhombus(std::make_unique<Object3D>()),
	mPointLight(std::make_unique<PointLight>())
{
	mRipple->SetModel(ModelManager::GetModel("Plane"));
	mRipple->SetTexture(TextureManager::GetTexture("DissolveTexture"));

	mRhombus->SetModel(ModelManager::GetModel("Plane"));
	mRhombus->SetTexture(TextureManager::GetTexture("BlurNoice"));

	mType = FieldObjectType::RespawnPoint;
}

void RespawnPoint::Init()
{
	mRipple->pos.y = 0.05f;
	mRipple->rot.x = Radian(90);
	mRipple->scale = 2.f;
	mRipple->color = Color(0xc4c178);
	mRipple->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Ripple"));

	mRhombus->pos.y = 1.5f;
	mRhombus->color = Color(0xc4c178);
	mRhombus->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Rhombus"));
	mRhombus->SetBillboardType(BillboardType::YAxisBillboard);

	mAngle.SetLimitTimer(360);

	EffectManager::GetInstance()->GenerateLeadEffect(mParent.pos + Vec3::up * 4.5f, Vec3::front + Vec3(0.f, 0.1f, 0.f));

	mPointLight->pos = mParent.pos + Vec3::up * 4.5f;
	mPointLight->radius = 15;
	mPointLight->decay = 5;
}

void RespawnPoint::Update()
{
	ColliderUpdatge();

	mRipple->offset.x -= 0.001f;
	mRhombus->offset.x += 0.01f;

	mRhombus->pos.y += cosf(Radian((float)mAngle.GetTimer())) * 0.0075f;
	mAngle.Update();
	if (mAngle == true)
	{
		mAngle.Reset();
	}

	mRipple->SetParent(&mParent);
	mRhombus->SetParent(&mParent);
	mRipple->Update();
	mRhombus->Update();
}

void RespawnPoint::Draw(const bool isDrawDepth)
{
	if (isDrawDepth == false)
	{
		mRipple->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Ripple"));
		mRhombus->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Rhombus"));
	}
	else
	{
		mRipple->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("RippleWriteNone"));
		mRhombus->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("RhombusWriteNone"));
	}

	mRipple->Draw("Object3D");
	mRhombus->Draw("Object3D");
}

void RespawnPoint::ColliderUpdatge()
{
	mCollider.centerPos = mParent.pos;
	mCollider.radius = mParent.scale.x;
}

void RespawnPoint::SetParent(const Transform parent)
{
	mParent = parent;
	mParent.Update();
}

Vec3 RespawnPoint::GetPos()
{
	return mParent.pos;
}

SphereCollider RespawnPoint::GetCollider()
{
	return mCollider;
}

