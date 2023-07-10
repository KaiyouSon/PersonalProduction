#include "Player.h"
#include "GaugeType.h"
#include "Club.h"

Player::Player() :
	player_(std::make_unique<HumanoidBody>()),
	weapon_(std::make_unique<Club>()),
	frontVec_(Vec3::front)
{
}

void Player::Init()
{
	GaugeParamInit();
	player_->Init();
	player_->pos.y = 4.5f;

	player_->SetWeapon(weapon_.get(), WeaponPartID::Right);
	player_->parent = this;

	joggingSpeed_ = 0.7f;
	runSpeed_ = 1.2f;

	pushTimer.SetLimitTimer(20);
}
void Player::PrevUpdate()
{
	moveVel = 0;

	// 関数ポインタ
	void (Player:: * pFunc[])() =
	{
		// 登録
		&Player::IdleUpdate,
		&Player::JoggingUpdate,
		&Player::RunUpdate,
		&Player::BackstepUpdate,
		&Player::RollUpdate,
		&Player::AttackR1Update,
		&Player::AttackR2Update,
	};

	// 実行
	(this->*pFunc[(int)state_])();

	GaugeParamUpdate();

	//player_->pos.y = 4.5f;
	player_->PrevUpdate();
}
void Player::PostUpdate()
{
	player_->PostUpdate();
}
void Player::DrawModel()
{
	player_->DrawModel();
}
void Player::DrawDebugGui()
{
	player_->DrawDebugGui();
}

// コライダー関連
void Player::CalcFrontVec()
{
	// カメラの前ベクトル
	Vec3 cameForward = player_->pos - Camera::current.pos;
	cameForward.y = 0.f;

	// カメラの右ベクトル
	Vec3 cameRight = Vec3::Cross(cameForward, Vec3::up);

	Vec3 stick =
	{
		Pad::GetStick(PadCode::LeftStick, 300).x,
		0,
		Pad::GetStick(PadCode::LeftStick, 300).y,
	};
	if (stick != 0)
	{
		Vec3 stickMoveVec = 0;

		stickMoveVec.x = -stick.Norm().x;
		stickMoveVec.z = -stick.Norm().z;

		frontVec_ = cameForward * stickMoveVec.z + cameRight * stickMoveVec.x;
	}
}
void Player::CalcBodyCollider()
{
	ColliderUpdate();
}
void Player::ColliderUpdate()
{
	bodyCollider_.startPos = player_->pos - Vec3(0.f, 2.5f, 0.f);
	bodyCollider_.endPos = player_->pos + Vec3(0.f, 2.5f, 0.f);
	bodyCollider_.radius = 2.5f;
}

// ゲージ関連
void Player::GaugeParamInit()
{
	// HPゲージ
	gaugePrames_[(uint32_t)GaugeType::Hp].CalcRate(256.f, 256.f);

	// MPゲージ
	gaugePrames_[(uint32_t)GaugeType::Mp].CalcRate(64.f, 64.f);

	// スタミナゲージ
	gaugePrames_[(uint32_t)GaugeType::Stamina].CalcRate(128.f, 128.f);
}
void Player::GaugeParamUpdate()
{
	if (state_ != State::AttackR1 && state_ != State::Run)
	{
		gaugePrames_[(uint32_t)GaugeType::Stamina].value++;
	}

	for (uint32_t i = 0; i < gaugePrames_.size(); i++)
	{
		gaugePrames_[i].Update();
	}
}

// ステート関連
void Player::MoveUpdate()
{
	CalcFrontVec();

	if (frontVec_ != 0)
	{
		moveVel = frontVec_.Norm() * moveSpeed_;

		player_->pos += moveVel;
		player_->rot.y = atan2f(frontVec_.x, frontVec_.z);
	}
}
void Player::IdleUpdate()
{
	player_->IdleMotion();

	if (Pad::GetButtonDown(PadCode::ButtonR1))
	{
		state_ = State::AttackR1;
	}
	else if (Pad::GetTrigger(PadCode::RightTrigger))
	{
		state_ = State::AttackR2;
	}
	else if (Pad::GetButtonDown(PadCode::ButtonB))
	{
		state_ = State::Backstep;
	}
	else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
	{
		if (Pad::GetButton(PadCode::ButtonB))
		{
			state_ = State::Run;
		}
		else
		{
			state_ = State::Jogging;
		}
	}
}
void Player::JoggingUpdate()
{
	player_->JoggingMotion();

	MoveUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonR1))
	{
		state_ = State::AttackR1;
	}
	else if (Pad::GetTrigger(PadCode::RightTrigger))
	{
		state_ = State::AttackR2;
	}
	else if (Pad::GetButton(PadCode::ButtonB))
	{
		// 何フレーム押したかを記録する
		pushTimer.Update(false);
		if (pushTimer.GetisTimeOut() == true)
		{
			state_ = State::Run;
			player_->ChangeMoveMotionInit();
			pushTimer.Reset();
		}
	}
	else if (player_->GetisPlayMoveMotion() == false)
	{
		state_ = State::Idle;
	}

	// 離した時
	if (Pad::GetButtonUp(PadCode::ButtonB))
	{
		if (pushTimer.GetisTimeOut() == false)
		{
			state_ = State::Roll;
			player_->RollMotionInit();
			player_->ChangeMoveMotionInit();
		}
		pushTimer.Reset();
	}
}
void Player::RunUpdate()
{
	player_->RunMotion();

	gaugePrames_[(uint32_t)GaugeType::Stamina].value -= 1.f;

	MoveUpdate();

	if (!Pad::GetButton(PadCode::ButtonB) ||
		gaugePrames_[(uint32_t)GaugeType::Stamina].value <= 0.f)
	{
		state_ = State::Jogging;
		player_->ChangeMoveMotionInit();
	}
	else if (player_->GetisPlayMoveMotion() == false)
	{
		state_ = State::Idle;
	}
}
void Player::BackstepUpdate()
{
	player_->BackstepMotionUpdate();

	if (player_->GetisBackStepMotionCanChange() == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonB))
		{
			player_->BackstepMotionInit();
		}
		else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			player_->BackstepMotionInit();
			player_->ChangeMoveMotionInit();

			if (Pad::GetButton(PadCode::ButtonB))
			{
				state_ = State::Roll;
			}
			else
			{
				state_ = State::Jogging;
			}
		}
	}
	else if (player_->GetisPlayBackStepMotion() == false)
	{
		state_ = State::Idle;
	}
}
void Player::RollUpdate()
{
	player_->RollMotionUpdate();

	if (player_->GetisRollMotionCanChange() == true)
	{
		if (Pad::GetButtonDown(PadCode::ButtonB))
		{
			state_ = State::Backstep;
			player_->RollMotionInit();
		}
		else if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			if (Pad::GetButtonDown(PadCode::ButtonB))
			{
				state_ = State::Roll;
				player_->RollMotionInit();
			}
			else
			{
				state_ = State::Jogging;
				player_->RollMotionInit();
			}
		}
	}

	if (player_->GetisPlayRollMotion() == false)
	{
		if (state_ == State::Roll)
		{
			state_ = State::Idle;
			player_->RollMotionInit();
		}
	}
}
void Player::AttackR1Update()
{
	player_->AttackR1MotionUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonB))
	{
		if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			player_->AttackMotionInit(0);
			state_ = State::Roll;
		}
		else
		{
			if (player_->GetisAttackMotionCanChange(0) == true)
			{
				player_->AttackMotionInit(0);
				state_ = State::Backstep;
			}
		}
	}

	if (player_->GetisPlayAttackMotion(0) == false)
	{
		if (state_ == State::AttackR1)
		{
			player_->AttackMotionInit(0);
			state_ = State::Idle;
		}
	}
}
void Player::AttackR2Update()
{
	player_->AttackR2MotionUpdate();

	if (Pad::GetButtonDown(PadCode::ButtonB))
	{
		if (Pad::GetStick(PadCode::LeftStick, 300) != 0)
		{
			player_->AttackMotionInit(0);
			state_ = State::Roll;
		}
		else
		{
			if (player_->GetisAttackMotionCanChange(0) == true)
			{
				player_->AttackMotionInit(0);
				state_ = State::Backstep;
			}
		}
	}

	if (player_->GetisPlayAttackMotion(0) == false)
	{
		if (state_ == State::AttackR2)
		{
			player_->AttackMotionInit(0);
			state_ = State::Idle;
		}
	}
}

// セッター
void Player::SetPos(const Vec3 pos)
{
	player_->pos = pos;
}

// ゲッター
GaugeParam Player::GetGaugeParam(const uint32_t index)
{
	return gaugePrames_[index];
}
HumanoidBody* Player::GetHumanoidBody()
{
	return player_.get();
}
Weapon* Player::GetWeapon()
{
	return weapon_.get();
}
Vec3 Player::GetPos()
{
	return player_->pos;
}
Vec3 Player::GetAttackPos()
{
	return player_->attackPos;
}
Vec3 Player::GetHeadPos()
{
	return player_->GetWorldPos(PartID::Head);
}
Vec3 Player::GetMoveVel()
{
	return moveVel;
}
Vec3 Player::GetFrontVec()
{
	return frontVec_;
}
Player::State Player::GetState()
{
	return state_;
}
CapsuleCollider Player::GetBodyCollider()
{
	return bodyCollider_;
}
