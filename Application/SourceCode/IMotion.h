#pragma once
#include "NewEngine.h"

class HumanoidBody;

class IMotion
{
protected:
	bool isInit_;
	bool isPlay_;

	uint32_t step_;
	Easing ease_;

	std::vector<Vec3> curRots_;
	std::vector<Vec3> endRots_;

public:
	virtual void Init(HumanoidBody* human) = 0;
	virtual void Update(HumanoidBody* human) = 0;

public:
	bool GetisPlay();
};

