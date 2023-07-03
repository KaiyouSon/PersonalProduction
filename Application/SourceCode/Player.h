#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "HumanoidBody.h"

class Player
{
public:
	enum class State
	{
		Idle,
		Jogging,
		Run,
		AttackR1,
	};

private:
	std::array<GaugeParam, 3> gaugePrames_;
	std::unique_ptr<Weapon> weapon_;

	std::unique_ptr<HumanoidBody> player_;

	Timer pushCoolTimer; //ボタン連打させないように

	State state_;

	float moveSpeed_;
	float joggingSpeed_;
	float runSpeed_;

private:
	void GaugeParamInit();
	void GaugeParamUpdate();

private:
	void MoveUpdate();

	void IdleUpdate();
	void JoggingUpdate();
	void RunUpdate();
	void AttackR1Update();

public:
	Player();
	void Init();
	void PrevUpdate();
	void PostUpdate();
	void DrawModel();
	void DrawDebugGui();

	void CalcBodyCollider();

public:
	void SetPos(const Vec3 pos);

public:
	GaugeParam GetGaugeParam(const uint32_t index);
	HumanoidBody* GetHumanoidBody();
	Weapon* GetWeapon();
	Vec3 GetPos();
	Vec3 GetAttackPos();
	Vec3 GetHeadPos();
	Vec3 GetVel();
	State GetState();

	friend HumanoidBody;
	friend MoveMotion;
};

