#pragma once
#include "NewEngine.h"

// メニューの種類
enum class MenuType
{
	LogoutMenu,
};

// メニューのインターフェース
class IMenu
{
public:
	virtual ~IMenu() {}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void DrawFrontSprite() = 0;
};
