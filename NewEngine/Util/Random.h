#pragma once
#include "Vec3.h"
#include <cstdint>

namespace Random
{
	void Init();

	// uint32_t型の乱数を取得
	int32_t Range(const int32_t min, const int32_t max);

	// float型の乱数を取得
	float RangeF(const float min, const float max);

	// Vec2型の乱数を取得
	Vec2 RangeVec2(const float min, const float max, const bool isSameValue = false);

	// Vec3型の乱数を取得
	Vec3 RangeVec3(const float min, const float max, const bool isSameValue = false);

	// 確率でtrueを返す
	bool RangeB(const float rate);

	// ランダムの角度を返す
	float RangeAngle(const float min = 1.f, const float max = 360.f);

	// ランダムのラジアンを返す
	float RangeRadian(const float min = 0.01745f, const float max = 6.28319f);

	// ランダムで円内の点を返す
	Vec2 RangeInCircle(const float minRadius, const float maxRadius);
}