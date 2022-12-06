#pragma once
#include "Vec3.h"
class Random
{
public:
	static void Initialize();
	static int Range(const int& min, const int& max);			// int型の乱数を取得
	static float RangeF(const float& min, const float& max);	// float型の乱数を取得
	static bool RangeB(const float& rate);						// 確率でtrueを返す
};