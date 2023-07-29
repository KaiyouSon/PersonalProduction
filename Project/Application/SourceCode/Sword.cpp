#include "Sword.h"
#include "MalletMotion.h"

Sword::Sword()
{
	weapon = std::make_unique<Object3D>();
	weapon->SetModel(ModelManager::GetModel("Sword"));

	// 槌のモーション
	motion = std::make_unique<MalletMotion>();

	localPos_ = Vec3(0.f, -1.5f, 0.f);

	isCalcCollider_ = false;
}

void Sword::Init()
{
}

void Sword::Update(Transform* parent)
{
	weapon->Update(parent);

	collider.startPos;
}

void Sword::DrawModel()
{
	weapon->Draw();
}

void Sword::ColliderUpdate()
{
	if (isCalcCollider_ == false)
	{
		collider.startPos = Vec3(0, -10000, 0);
		collider.endPos = Vec3(0, -10000, 0);
		collider.radius = 0.f;
	}
	else
	{
		Vec3 zAxis = weapon->GetTransform().GetWorldMat().GetZAxis();

		collider.startPos = weapon->GetWorldPos() - zAxis.Norm() * 8.f;
		collider.endPos = weapon->GetWorldPos() + zAxis.Norm() * 8.f;
		collider.radius = 3.f;
	}
}

void Sword::CalcDamage()
{
}
