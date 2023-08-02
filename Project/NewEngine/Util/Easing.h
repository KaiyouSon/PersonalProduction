#pragma once
#include "Vec3.h"
#include "Enum.h"
#include <cstdint>

class Easing
{
private:
	int32_t mTimer;
	int32_t mLimitTimer;
	float mTimeRate;
	float mPowNum;
	bool mIsEnd;
	EaseType mEaseType;

public:
	Easing();
	Easing(const int32_t limitTimer);
	Easing(const int32_t limitTimer, const float powNum);
	void Reset();		// リセット
	void Update();		// 補間

public: // セッター

	// 補間時間をセットする
	inline void SetEaseTimer(const int32_t limitTimer) { mLimitTimer = limitTimer; }
	// N乗をセットする
	inline void SetPowNum(const float powNum) { mPowNum = powNum; }
	inline void SetEaseType(const EaseType easeType) { mEaseType = easeType; }
	inline void SetisEnd(const bool isEnd) { mIsEnd = isEnd; }

public: // ゲッター

	// イージング終わり
	inline bool GetisEnd() { return mIsEnd; }
	// タイマー取得
	inline int32_t GetTimer() { return mTimer; }
	// タイムレートを取得
	inline float GetTimeRate() { return mTimeRate; }

public:

	float Interpolation(const float startPos, const float endPos);
	Vec2 Interpolation(const Vec2 startPos, const Vec2 endPos);
	Vec3 Interpolation(const Vec3 startPos, const Vec3 endPos);

	// ラープ
	float Lerp(const float startPos, const float endPos);
	Vec2 Lerp(const Vec2 startPos, const Vec2 endPos);
	Vec3 Lerp(const Vec3 startPos, const Vec3 endPos);

	// イーズイン
	float In(const float startPos, const float endPos);
	Vec2 In(const Vec2 startPos, const Vec2 endPos);
	Vec3 In(const Vec3 startPos, const Vec3 endPos);

	// イーズアウト
	float Out(const float startPos, const float endPos);
	Vec2 Out(const Vec2 startPos, const Vec2 endPos);
	Vec3 Out(const Vec3 startPos, const Vec3 endPos);

	// インアウト
	float InOut(const float startPos, const float endPos);
	Vec2 InOut(const Vec2 startPos, const Vec2 endPos);
	Vec3 InOut(const Vec3 startPos, const Vec3 endPos);

	// インバック
	float InBack(const float startPos, const float endPos);
	Vec2 InBack(const Vec2 startPos, const Vec2 endPos);
	Vec3 InBack(const Vec3 startPos, const Vec3 endPos);
};
