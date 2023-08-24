#include "FrameRate.h"
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

FrameRate::FrameRate() : mFrameRate(0)
{
}

void FrameRate::Init(const float frameRate)
{
	mReference = steady_clock::now();
	mFrameRate = frameRate;
}

void FrameRate::Update()
{
	// １/ mFrameRate秒ぴったりの時間
	const microseconds minTime(uint64_t(1000000.0f / mFrameRate));

	// １/ mFrameRate秒よりわずかに短い時間
	const microseconds minCheckTime(uint64_t(1000000.0f / (mFrameRate + 5)));

	// 現在の時間を取得
	steady_clock::time_point nowTime = steady_clock::now();

	// 前回の記録から経過時間を取得する
	microseconds elapsedTime = duration_cast<microseconds>(nowTime - mReference);

	// １/mFrameRate秒経ってない場合
	if (elapsedTime < minCheckTime)
	{
		// １/mFrameRate秒経過するまで微小なスリープを繰り返す
		while (steady_clock::now() - mReference < minTime)
		{
			sleep_for(microseconds(1));
			nowTime = steady_clock::now();
		}
	}

	// 現在のFPS
	mCurrentFPS = 1.0f / static_cast<float>(elapsedTime.count() / 1000000.0f);

	// 現在の時間を記録する
	mReference = steady_clock::now();
}

float FrameRate::GetCurrentFPS()
{
	return mCurrentFPS;
}
