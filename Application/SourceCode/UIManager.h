#pragma once
#include "GaugeUI.h"
#include "Player.h"
#include "Boss.h"

class UIManager
{
private:
	std::array<std::unique_ptr<GaugeUI>, 3> gauges_;
	std::unique_ptr<GaugeUI> bossHPGauge_;

	Player* player_;
	Boss* boss_;

public:
	UIManager();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetPlayer(Player* player);
	void SetBoss(Boss* boss);
};

