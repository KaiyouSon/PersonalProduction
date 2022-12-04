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

class Util
{
public:
	// 比較して大きい方を返す
	static float Max(float a, float b);

	// 比較して小さい方を返す
	static float Min(float a, float b);

	// 符号を返す（ -1, 0, 1 ）
	static int Sign(float a);
};