#pragma once
#include "Color.h"
#include "FrameRate.h"
#include "Random.h"
#include "Singleton.h"
#include "MathUtil.h"
#include "Transform.h"
#include "Camera.h"
#include "SceneManager.h"
#include <memory>
#include <vector>
#include <string>
#include <list>

enum BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv
};

// 比較して大きい方を返す
float Max(const float& a, const float& b);

// 比較して小さい方を返す
float Min(const float& a, const float& b);

// 符号を返す（ -1, 0, 1 ）
int Sign(const float& a);

// 値を制限する関数
float Clamp(const float& value, const float& min = 0, const float& max = 1);

// 桁数を取得
int GetDight(const int& value);
