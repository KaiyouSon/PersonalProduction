#pragma once
#include "IMotion.h"

class BackstepMotion : public IMotion
{
private:
	Easing moveEase_;
	Vec3 startPos_;
	Vec3 endPos_;
	float length_;
	float up_;
	float down_;

private:
	void CurrentStepInit(HumanoidBody* human);
	void CurrentStepUpdate(HumanoidBody* human);

	void Step0Init(HumanoidBody* human);
	void Step0Update(HumanoidBody* human);
	void Step1Update(HumanoidBody* human);
	void Step2Update(HumanoidBody* human);

public:
	BackstepMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
};

