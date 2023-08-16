#include "ColliderDrawer.h"
#include "ModelManager.h"
#include "Collider.h"

uint32_t ColliderDrawer::index = 0;

void ColliderDrawer::Load()
{
	// コライダーモデルのロード
	ModelManager::LoadObjModel("Collider/SphereCollider", "SphereCollider");
	ModelManager::LoadObjModel("Collider/CapsuleColliderParts1", "CapsuleColliderParts1");
	ModelManager::LoadObjModel("Collider/CapsuleColliderParts2", "CapsuleColliderParts2");

	// 球体
	mModels.insert(std::make_pair("Sphere", ModelManager::GetModel("SphereCollider")));
	mModels.insert(std::make_pair("CapsuleColliderParts1", ModelManager::GetModel("CapsuleColliderParts1")));
	mModels.insert(std::make_pair("CapsuleColliderParts2", ModelManager::GetModel("CapsuleColliderParts2")));

}

void ColliderDrawer::Init()
{
	mMaxColliderNum = 64;

	for (uint32_t i = 0; i < mMaxColliderNum; i++)
	{
		mColliderObjects.push_back(std::make_unique<ColliderObject>());
	}
}

void ColliderDrawer::Update()
{
	for (const auto& colliderObjcet : mColliderObjects)
	{
		colliderObjcet->Update();
	}
}

void ColliderDrawer::DrawCollider()
{
	for (const auto& colliderObjcet : mColliderObjects)
	{
		colliderObjcet->Draw();
	}

	// 次のフレームの準備
	index = 0;
}

void ColliderDrawer::Bind(ICollider* collider)
{
	if (index >= mMaxColliderNum)
	{
		OutputDebugLog("MaxColliderNumの数が足りません");
		return;
	}

	if (collider == nullptr)
	{
		OutputDebugLog("BindしているColliderがnullptrです");

		return;
	}

	// コライダーを構成するために必要な数分格納する
	switch (collider->primitive)
	{
	case ColliderPrimitive::Sphere:
	{
		// キャスト
		SphereCollider* castCollider = dynamic_cast<SphereCollider*>(collider);
		mColliderObjects[index]->model = mModels["Sphere"];
		mColliderObjects[index]->transform.pos = castCollider->centerPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot = 0;
		index++;
	}
	break;

	case ColliderPrimitive::Capsule:
	{
		// キャスト
		CapsuleCollider* castCollider = dynamic_cast<CapsuleCollider*>(collider);

		// 下の半球
		mColliderObjects[index]->model = mModels["CapsuleColliderParts1"];
		mColliderObjects[index]->transform.pos = castCollider->startPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot.z = Radian(180);
		index++;

		// 上半球
		mColliderObjects[index]->model = mModels["CapsuleColliderParts1"];
		mColliderObjects[index]->transform.pos = castCollider->endPos;
		mColliderObjects[index]->transform.scale = castCollider->radius;
		mColliderObjects[index]->transform.rot = 0;
		index++;

		// 円柱
		Vec3 v = castCollider->endPos - castCollider->startPos;
		Vec3 halfPos = v / 2;
		float length = v.Length() / 2;

		mColliderObjects[index]->model = mModels["CapsuleColliderParts2"];
		mColliderObjects[index]->transform.pos = castCollider->startPos + halfPos;
		mColliderObjects[index]->transform.scale = Vec3(castCollider->radius, length, castCollider->radius);
		mColliderObjects[index]->transform.rot = 0;
		index++;
	}
	break;

	}

}
