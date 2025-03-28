#pragma once
#include "BloodSprayEffect.h"
#include "IEffect.h"
#include "Player.h"

template<typename T> class Singleton;

// 全てのエフェクトを管理するクラス
class EffectManager : public Singleton<EffectManager>
{
public:
	enum class SmokeEffectIndex
	{
		PlayerUse,
		LeftGateUse,
		RightGateUse,
		Size,
	};

private:
	std::vector<std::unique_ptr<IEffect>> mEffects;
	std::vector<std::unique_ptr<IEffect>> mSmokeEffects;

	std::unique_ptr<BloodSprayEffect> mBloodSprayEffect;


private:
	Player* mPlayer;

public:
	EffectManager();
	void Init();
	void Update();
	void ExecuteCS();
	void DrawEffect(const bool isBloom = false);

public:
	void GenerateAirEffect(const Vec3 pos);
	void GenerateAttackExplosionEffect(const Vec3 pos);
	void GenerateBloodSprayEffect(const Vec3 pos);
	void GeneratePlayerRecoveryEffect(const Vec3 pos);
	void GenerateLeadEffect(const Vec3 pos, const Vec3 frontVec);
	void GenerateLogoExplosionEffect(const Vec3 pos, const Vec3 rot, const Vec3 scale);
	void GenerateBossAttackTrajectoryEffect();
	void GenerateSmokeEffect();

public:
	void ExplosionLogoExplosionEffect();
	void ExecuteBossAttackTrajectoryEffect(const bool isGenerate, const Vec3 startPos, const Vec3 endPos);
	void ExecuteSmokeEffect(const ConstantBufferData::CSmokeEffect data, const SmokeEffectIndex index);
	void ExecuteAirEffect(const Vec3 pos);

public:
	void SetPlayer(Player* player);

private:
	friend Singleton<EffectManager>;
};
