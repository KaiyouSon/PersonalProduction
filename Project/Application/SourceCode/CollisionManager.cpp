#include "CollisionManager.h"
#include "EffectManager.h"
#include "TransitionManager.h"

void CollisionManager::Update()
{
	// プレイヤーとボス
	PlayerHitBoss();

	if (mMovieEvent->GetisPlaying() == false)
	{
		PlayerHitFieldObject();

		PlayerHitNegotiation();

		PlayerHitAirCollider();
	}

	// ボスとプレイヤー
	BossHitPlayer();
}

void CollisionManager::PlayerHitBoss()
{
	if (mPlayer->GetisAlive() == false)
	{
		return;
	}

	static Vec3 hitPoint = {};

	float dis = Vec3::Distance(mPlayer->GetPos(), mBoss->GetPos());
	if (dis >= 50)
	{
		return;
	}

	bool isAttackBoss =
		Collision::CapsuleHitCapsule(
			mPlayer->GetWeapon()->collider,
			mBoss->GetCollider(),
			hitPoint);

	if (isAttackBoss)
	{
		if (mBoss->GetisAlive() == true)
		{
			if (mBoss->GetisDamage() == false)
			{
				mBoss->Damage(mPlayer->GetWeapon()->GetDamage());

				SoundManager::Play("HitSE");
				EffectManager::GetInstance()->GenerateBloodSprayEffect(hitPoint);

				mBoss->SetisDamage(true);
			}
		}
	}

	bool isBodyTouch =
		Collision::CapsuleHitCapsule(
			mPlayer->GetBodyCollider(),
			mBoss->GetCollider());

	if (isBodyTouch == true)
	{
		// 半径を足して実際の長さを求める
		float checkLength =
			mPlayer->GetBodyCollider().radius +
			mBoss->GetCollider().radius;

		// y軸を無視する
		Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = mBoss->GetPos() * Vec3(1, 0, 1);

		Vec3 toPlayer = pos1 - pos2;

		// 現在の長さ
		float curLength = toPlayer.Length();

		// checkLength と curLength の差分で埋め込んだ長さが求まる
		float embedLength = checkLength - curLength;

		// 誤差をけす処理
		Vec3 normal = toPlayer.Norm();
		if (fabs(curLength) < 0.0001f)
		{
			normal = 0;
		}

		Vec3 pushVec = normal * embedLength;

		Vec3 nextPos = mPlayer->GetPos() + pushVec;
		mPlayer->SetPos(nextPos);
	}
}
void CollisionManager::PlayerHitNegotiation()
{
	// 何かに当たった時用
	bool isHit = false;

	// リスポーン地点と当たったら
	for (const auto& respawnPoint : mField->GetFieldData()->respawnPoints)
	{
		float dis = Vec3::Distance(mPlayer->GetPos(), respawnPoint->GetPos());
		if (dis >= 20)
		{
			continue;
		}

		if (Collision::SphereHitCapsule(
			respawnPoint->GetCollider(), mPlayer->GetBodyCollider()))
		{
			mUiManager->GetNegotiationUI()->SetisActive(true);
			mUiManager->GetNegotiationUI()->SetTextType(NegotiationUI::TextType::RestInLightText);
			isHit = true;

			if (Pad::GetButtonDown(PadCode::ButtonB))
			{
				TransitionManager::GetInstance()->Start(TransitionType::Respawn);
			}
			break;
		}
	}

	// フィールドデータ
	FieldData* fieldData = mField->GetFieldData();

	// 当たり判定の有効距離
	const float effectiveDis = 50;

	// フィールドオブジェクト
	const auto& fieldObjects = fieldData->mFieldObjects;
	for (const auto& obj : fieldObjects)
	{
		switch (obj->GetType())
		{
		case FieldObjectType::Gate:
		{
			// キャスト
			Gate* gate = dynamic_cast<Gate*>(obj.get());

			// 距離が遠かったら当たり判定取らないように
			float dis = Vec3::Distance(mPlayer->GetPos(), gate->GetCenterPos());
			if (dis >= effectiveDis)
			{
				continue;
			}

			if (gate->GetisOpen() == true)
			{
				continue;
			}

			if (Collision::SphereHitCapsule(
				gate->GetNegotiationCollider(), mPlayer->GetBodyCollider()))
			{
				mUiManager->GetNegotiationUI()->SetisActive(true);
				mUiManager->GetNegotiationUI()->SetTextType(NegotiationUI::TextType::OpenText);
				isHit = true;

				if (Pad::GetButtonDown(PadCode::ButtonB))
				{
					Vec3 pos = gate->GetNegotitationPos();
					pos.y = mPlayer->GetPos().y;
					mPlayer->SetPos(pos);

					mPlayer->SetState(Player::State::OpenGate);

					gate->SetisOpening(true);
				}
			}
		}
		break;
		}

		if (isHit == true)
		{
			break;
		}
	}

	// 何も当たってなかった時
	if (isHit == false)
	{
		mUiManager->GetNegotiationUI()->SetisActive(false);
	}
}
void CollisionManager::PlayerHitFieldObject()
{
	// フィールドデータ
	FieldData* fieldData = mField->GetFieldData();

	// 当たり判定の有効距離
	const float effectiveDis = 50;

	// フィールドオブジェクト
	const auto& fieldObjects = fieldData->mFieldObjects;
	for (const auto& obj : fieldObjects)
	{
		switch (obj->GetType())
		{

		case FieldObjectType::Coffin:
		case FieldObjectType::PlayerCoffin:
		{
			// キャスト
			Coffin* coffin = dynamic_cast<Coffin*>(obj.get());

			// 距離が遠かったら当たり判定取らないように
			float dis = Vec3::Distance(mPlayer->GetPos(), coffin->GetPos());
			if (dis >= effectiveDis)
			{
				continue;
			}

			// 押し戻しの処理
			CubeCollisionPlayer(coffin->GetBottomCollider());
		}
		break;

		case FieldObjectType::Wall:
		{
			// キャスト
			Wall* wall = dynamic_cast<Wall*>(obj.get());

			// 距離が遠かったら当たり判定取らないように
			float dis = Vec3::Distance(mPlayer->GetPos(), wall->GetPos());
			if (dis >= effectiveDis)
			{
				continue;
			}

			// 押し戻しの処理
			CapsuleCollisionPlayer(wall->GetCollider());
		}
		break;

		case FieldObjectType::Gate:
		{
			// キャスト
			Gate* gate = dynamic_cast<Gate*>(obj.get());

			// 距離が遠かったら当たり判定取らないように
			float dis = Vec3::Distance(mPlayer->GetPos(), gate->GetCenterPos());
			if (dis >= effectiveDis)
			{
				continue;
			}

			// 押し戻しの処理
			CapsuleCollisionPlayer(gate->GetLeftCollider());
			CapsuleCollisionPlayer(gate->GetRightCollider());
			CapsuleCollisionPlayer(gate->GetCloseCollider());
		}
		break;

		}
	}

	// 木
	const std::vector<std::unique_ptr<Tree>>& trees = fieldData->trees;
	for (uint32_t i = 0; i < trees.size(); i++)
	{
		float dis = Vec3::Distance(mPlayer->GetPos(), trees[i]->GetPos());
		if (dis >= 20)
		{
			continue;
		}

		Vec3 hitPoint = 0;
		if (Collision::SphereHitCapsule(
			trees[i]->GetCollider(), mPlayer->GetBodyCollider(), hitPoint))
		{
			// 半径を足して実際の長さを求める
			float checkLength =
				mPlayer->GetBodyCollider().radius + trees[i]->GetCollider().radius;

			// y軸を無視する
			Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
			Vec3 pos2 = trees[i]->GetPos() * Vec3(1, 0, 1);

			Vec3 toPlayer = pos1 - pos2;

			// 現在の長さ
			float curLength = toPlayer.Length();

			// checkLength と curLength の差分で埋め込んだ長さが求まる
			float embedLength = checkLength - curLength;

			// 誤差をけす処理
			Vec3 normal = toPlayer.Norm();
			if (fabs(curLength) < 0.0001f)
			{
				normal = 0;
			}

			Vec3 pushVec = normal * embedLength;

			Vec3 nextPos = mPlayer->GetPos() + pushVec;
			mPlayer->SetPos(nextPos);
		}
	}

	// 正門
	//const std::vector<std::unique_ptr<Gate>>& gates = fieldData->gates;
	//for (uint32_t i = 0; i < gates.size(); i++)
	//{
	//	float dis = Vec3::Distance(mPlayer->GetPos(), gates[i]->GetCenterPos());
	//	if (dis >= 50)
	//	{
	//		continue;
	//	}

	//	Vec3 hitPoint = 0;
	//	if (Collision::CapsuleHitCapsule(gates[i]->GetLeftCollider(), mPlayer->GetBodyCollider(), hitPoint) ||
	//		Collision::CapsuleHitCapsule(gates[i]->GetRightCollider(), mPlayer->GetBodyCollider(), hitPoint) ||
	//		Collision::CapsuleHitCapsule(gates[i]->GetCloseCollider(), mPlayer->GetBodyCollider(), hitPoint))
	//	{
	//		// y軸を無視する
	//		Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
	//		Vec3 pos2 = hitPoint * Vec3(1, 0, 1);

	//		// プレイヤーに向かうベクトル
	//		Vec3 toPlayer = pos1 - pos2;
	//		float toPlayerLength = toPlayer.Length();
	//		Vec3 normal = toPlayer.Norm();

	//		// 衝突した位置とプレイヤーの中心が重なる距離を計算
	//		float overlap = mPlayer->GetBodyCollider().radius - toPlayerLength;

	//		// 押し戻しのベクトル
	//		Vec3 pushVec = normal * (overlap);

	//		Vec3 nextPos = mPlayer->GetPos() + pushVec;
	//		mPlayer->SetPos(nextPos);
	//	}
	//}
}
void CollisionManager::PlayerHitAirCollider()
{
	FieldData* fieldData = mField->GetFieldData();

	// 当たり判定
	const auto& airColliders = fieldData->airColliders;
	for (uint32_t i = 0; i < airColliders.size(); i++)
	{
		Vec3 hitPoint = 0;
		if (Collision::CapsuleHitCapsule(
			airColliders[i]->GetCollider(), mPlayer->GetBodyCollider(), hitPoint))
		{
			// y軸を無視する
			Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
			Vec3 pos2 = hitPoint * Vec3(1, 0, 1);

			// プレイヤーに向かうベクトル
			Vec3 toPlayer = pos1 - pos2;
			float toPlayerLength = toPlayer.Length();
			Vec3 normal = toPlayer.Norm();

			// 衝突した位置とプレイヤーの中心が重なる距離を計算
			float overlap = mPlayer->GetBodyCollider().radius - toPlayerLength;

			// 押し戻しのベクトル
			Vec3 pushVec = normal * (overlap);

			Vec3 nextPos = mPlayer->GetPos() + pushVec;
			mPlayer->SetPos(nextPos);
		}
	}
}
void CollisionManager::BossHitPlayer()
{
	if (mPlayer->GetisAlive() == false ||
		mBoss->GetisAlive() == false)
	{
		return;
	}

	static Vec3 hitPoint = {};

	bool isAttackBoss =
		Collision::CapsuleHitCapsule(
			mBoss->GetWeapon()->collider,
			mPlayer->GetBodyCollider(),
			hitPoint);

	if (isAttackBoss)
	{
		if (mPlayer->GetState() != Player::State::Roll &&
			mPlayer->GetState() != Player::State::Backstep)
		{
			if (mPlayer->GetisDamage() == false)
			{
				SoundManager::Play("HitSE");
				mPlayer->Damage(mBoss->GetDamage() * 1.5f);
				EffectManager::GetInstance()->GenerateBloodSprayEffect(hitPoint);

				mPlayer->SetisDamage(true);
			}
		}
	}
}

// 押し戻しの処理
void CollisionManager::CubeCollisionPlayer(const CubeCollider& cube)
{
	Vec3 hitPoint = 0;
	if (Collision::CubeHitCapsule(cube, mPlayer->GetBodyCollider(), hitPoint))
	{
		// y軸を無視する
		Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = hitPoint * Vec3(1, 0, 1);

		// プレイヤーに向かうベクトル
		Vec3 toPlayer = pos1 - pos2;
		float toPlayerLength = toPlayer.Length();
		Vec3 normal = toPlayer.Norm();

		// 衝突した位置とプレイヤーの中心が重なる距離を計算
		float overlap = mPlayer->GetBodyCollider().radius - toPlayerLength;

		// 押し戻しのベクトル
		Vec3 pushVec = normal * (overlap);

		Vec3 nextPos = mPlayer->GetPos() + pushVec;
		mPlayer->SetPos(nextPos);
	}
}
void CollisionManager::CapsuleCollisionPlayer(const CapsuleCollider& capsule)
{
	Vec3 hitPoint = 0;
	if (Collision::CapsuleHitCapsule(capsule, mPlayer->GetBodyCollider(), hitPoint))
	{
		// y軸を無視する
		Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = hitPoint * Vec3(1, 0, 1);

		// プレイヤーに向かうベクトル
		Vec3 toPlayer = pos1 - pos2;
		float toPlayerLength = toPlayer.Length();
		Vec3 normal = toPlayer.Norm();

		// 衝突した位置とプレイヤーの中心が重なる距離を計算
		float overlap = mPlayer->GetBodyCollider().radius - toPlayerLength;

		// 押し戻しのベクトル
		Vec3 pushVec = normal * (overlap);

		Vec3 nextPos = mPlayer->GetPos() + pushVec;
		mPlayer->SetPos(nextPos);
	}
}


bool CollisionManager::IsCheckFrontBoss(const Vec3 pos, const Vec3 front)
{
	SphereCollider frontCollider;
	frontCollider.centerPos = pos + front;
	frontCollider.radius = 1.0f;

	SphereCollider bossCollider;
	bossCollider.centerPos = mBoss->GetPos();
	bossCollider.radius = 1.0f;

	bool isHit = Collision::SphereHitSphere(frontCollider, bossCollider);
	return isHit;
}
float CollisionManager::CalcPlayerDisToFront(const Vec3 frontVec, const float max)
{
	RayCollider playerFront;
	playerFront.startPos = mPlayer->GetPos();
	playerFront.dirVec = frontVec;

	SphereCollider bossCollider;
	bossCollider.centerPos = mBoss->GetPos();
	bossCollider.radius = 1.f;

	bool isHit = Collision::RayHitSphere(playerFront, bossCollider);

	float dis = max;
	if (isHit == true)
	{
		// y軸を無視する
		Vec3 pos1 = mPlayer->GetPos() * Vec3(1, 0, 1);
		Vec3 pos2 = mBoss->GetPos() * Vec3(1, 0, 1);

		float radius =
			mPlayer->GetBodyCollider().radius +
			mBoss->GetCollider().radius;
		dis = Vec3::Distance(pos1, pos2) - radius;
		dis = Min<float>(dis, max);
	}

	return dis;
}

void CollisionManager::SetPlayer(Player* player)
{
	mPlayer = player;
}
void CollisionManager::SetBoss(Boss* boss)
{
	mBoss = boss;
}
void CollisionManager::SetField(Field* field)
{
	mField = field;
}
void CollisionManager::SetUIManager(UIManager* uiManager)
{
	mUiManager = uiManager;
}

void CollisionManager::SetMovieEvent(MovieEvent* movieEvent)
{
	mMovieEvent = movieEvent;
}
