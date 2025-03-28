#include "Gate.h"
#include "EffectManager.h"

Gate::Gate() :
	mGateLeft(std::make_unique<Object3D>()),
	mGateRight(std::make_unique<Object3D>())
{
	mGateLeft->SetModel(ModelManager::GetModel("WallGate"));
	mGateRight->SetModel(ModelManager::GetModel("WallGate"));

	mType = FieldObjectType::Gate;
}
void Gate::Init()
{
	mGateLeft->SetisShadow(true, true);
	mGateRight->SetisShadow(true, true);
	mGateLeft->isLighting = true;
	mGateRight->isLighting = true;

	mLeftCollider.isActive = false;
	mRightCollider.isActive = false;
	mCloseCollider.isActive = true;
	mNegotiationCollider.isActive = false;
	mIsOpen = false;

	mOpenEase.SetEaseTimer(180);
	mStayTimer.SetLimitTimer(30);

	mGateLeft->rot.y = 0;
	mGateRight->rot.y = Radian(180);
}
void Gate::Update()
{
	mNegotitationPos = mCenterPos + Vec3::back * 7.5f;

	ColliderUpdate();
	OpeningUpdate();

	mGateLeft->Update();
	mGateRight->Update();
}
void Gate::Draw(const bool isDrawDepth)
{
	if (isDrawDepth == false)
	{
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
	}
	else
	{
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	}

	mGateLeft->Draw("Object3D");
	mGateRight->Draw("Object3D");
}

void Gate::ColliderUpdate()
{
	const float endLength = 20.f;

	const Vec3 leftVec = { cosf(mGateLeft->rot.y), 0, -sinf(mGateLeft->rot.y) };
	mLeftCollider.startPos = mGateLeft->pos;
	mLeftCollider.endPos = mGateLeft->pos + leftVec.Norm() * endLength;
	mLeftCollider.radius = 5.f;

	const Vec3 rightVec = { cosf(mGateRight->rot.y), 0, -sinf(mGateRight->rot.y) };
	mRightCollider.startPos = mGateRight->pos;
	mRightCollider.endPos = mGateRight->pos + rightVec.Norm() * endLength;
	mRightCollider.radius = 5.f;

	mCloseCollider.startPos = mGateLeft->pos;
	mCloseCollider.endPos = mGateRight->pos;
	mCloseCollider.radius = 5.f;

	if (mIsOpen == true)
	{
		mLeftCollider.isActive = true;
		mRightCollider.isActive = true;
		mCloseCollider.isActive = false;
		mNegotiationCollider.isActive = false;
	}
	else
	{
		mLeftCollider.isActive = false;
		mRightCollider.isActive = false;
		mCloseCollider.isActive = true;
		mNegotiationCollider.isActive = true;
	}

	ColliderDrawer::GetInstance()->Bind(&mLeftCollider);
	ColliderDrawer::GetInstance()->Bind(&mRightCollider);
	ColliderDrawer::GetInstance()->Bind(&mCloseCollider);
	ColliderDrawer::GetInstance()->Bind(&mNegotiationCollider);
}
void Gate::OpeningUpdate()
{
	// 煙のエフェクトのデータ
	ConstantBufferData::CSmokeEffect smokeEffectData;


	if (mIsOpening == false)
	{
		//smokeEffectData.isGenerate = false;
		//
		//EffectManager::GetInstance()->ExecuteSmokeEffect(
		//	smokeEffectData, mGateLeft->rot, EffectManager::SmokeEffectIndex::LeftGateUse);
		//EffectManager::GetInstance()->ExecuteSmokeEffect(
		//	smokeEffectData, mGateRight->rot, EffectManager::SmokeEffectIndex::RightGateUse);
		return;
	}

	mStayTimer.Update();
	if (mStayTimer == true)
	{
		mGateLeft->rot.y = mOpenEase.Lerp(Radian(0), Radian(-45));
		mGateRight->rot.y = mOpenEase.Lerp(Radian(180), Radian(225));


		// ----------------------------------------------------------------------------------------------------------------- //
		// --- エフェクトの処理 -------------------------------------------------------------------------------------------- //
		// ----------------------------------------------------------------------------------------------------------------- //
		//{
		//	smokeEffectData.isGenerate = true;
		//	smokeEffectData.generateTimer = 1;
		//	smokeEffectData.generateNum = 32;

		//	// 左の門の煙
		//	Vec3 leftVec = Vec3(cosf(mGateLeft->rot.y), 0, sinf(mGateLeft->rot.y));
		//	smokeEffectData.generatePos = mGateLeft->pos + leftVec * mGateLeft->scale.x * 2;
		//	smokeEffectData.generateRange.x = leftVec.x * 15.f;
		//	smokeEffectData.generateRange.z = leftVec.z * 6.f;

		//	static float rad = 0;
		//	rad += Radian(5);
		//	EffectManager::GetInstance()->ExecuteSmokeEffect(
		//		smokeEffectData, EffectManager::SmokeEffectIndex::LeftGateUse);

		//	// 右の門の煙
		//	Vec3 rightVec = Vec3(cosf(mGateRight->rot.y), 0, sinf(mGateRight->rot.y));
		//	smokeEffectData.generatePos = mGateRight->pos + rightVec * mGateRight->scale.x * 2;
		//	smokeEffectData.generateRange = leftVec.x * 15.f;
		//	smokeEffectData.generateRange = leftVec.z * 6.f;
		//	EffectManager::GetInstance()->ExecuteSmokeEffect(
		//		smokeEffectData, EffectManager::SmokeEffectIndex::RightGateUse);
		//}
		// ----------------------------------------------------------------------------------------------------------------- //
		// ----------------------------------------------------------------------------------------------------------------- //
		// ----------------------------------------------------------------------------------------------------------------- //



		mOpenEase.Update();
		if (mOpenEase.GetisEnd() == true)
		{
			mIsOpen = true;
			mIsOpening = false;
			mOpenEase.Reset();
			mStayTimer.Reset();
		}
	}
}

// セッター
void Gate::SetLeftTransform(const Transform& transform)
{
	mGateLeft->pos = transform.pos;
	mGateLeft->scale = transform.scale;
	mGateLeft->rot = transform.rot;
}
void Gate::SetRightTransform(const Transform& transform)
{
	mGateRight->pos = transform.pos;
	mGateRight->scale = transform.scale;
	mGateRight->rot = transform.rot;
}
void Gate::SetNegotiationCollider(const SphereCollider collider)
{
	mNegotiationCollider = collider;
	mNegotiationCollider.isActive = true;
}
void Gate::SetCenterPos(const Vec3 pos)
{
	mCenterPos = pos;
}
void Gate::SetisOpening(const bool isOpening)
{
	mIsOpening = isOpening;
}
void Gate::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGateLeft->SetGraphicsPipeline(graphicsPipeline);
	mGateRight->SetGraphicsPipeline(graphicsPipeline);
}

// ゲッター
CapsuleCollider Gate::GetLeftCollider()
{
	return mLeftCollider;
}
CapsuleCollider Gate::GetRightCollider()
{
	return mRightCollider;
}
CapsuleCollider Gate::GetCloseCollider()
{
	return mCloseCollider;
}
SphereCollider Gate::GetNegotiationCollider()
{
	return mNegotiationCollider;
}
Vec3 Gate::GetCenterPos()
{
	return mCenterPos;
}
Vec3 Gate::GetNegotitationPos()
{
	return mNegotitationPos;
}
bool Gate::GetisOpen()
{
	return mIsOpen;
}

