#include "BezierCurve.h"
#include "Easing.h"
using namespace std;

BezierCurve::BezierCurve() :
	mTimer(0), mTimeRate(0), mIsEnd(false), mStartIndex(1)
{
	mEase.SetEaseTimer(0);
	mEase.SetPowNum(1);
}

BezierCurve::BezierCurve(const int32_t mEaseTimer) :
	mTimer(0), mTimeRate(0), mIsEnd(false), mStartIndex(1)
{
	mEase.SetEaseTimer(mEaseTimer);
	mEase.SetPowNum(1);
}

BezierCurve::BezierCurve(const int32_t mEaseTimer, const float powNum) :
	mTimer(0), mTimeRate(0), mIsEnd(false), mStartIndex(1)
{
	mEase.SetEaseTimer(mEaseTimer);
	mEase.SetPowNum(powNum);
}

void BezierCurve::Reset()
{
	mTimer = 0;
	mTimeRate = 0;
	mEase.Reset();
	mIsEnd = false;
}

void BezierCurve::Update()
{
	if (mPoints.size() < 2) return;

	mEase.Update();
	if (mEase.GetisEnd() == true)
	{
		mEase.Reset();
		mIsEnd = true;
	}
}

Vec3 BezierCurve::InterPolation(const BezierType type)
{
	if (RecursiveLerp(mPoints, type).size() < 2) return { 0,0,0 };

	// 莠悟屓
	switch (type)
	{
	case BezierType::Lerp:
		return mEase.Lerp(RecursiveLerp(mPoints, type)[0], RecursiveLerp(mPoints, type)[1]);
		break;
	case BezierType::EaseIn:
		return mEase.In(RecursiveLerp(mPoints, type)[0], RecursiveLerp(mPoints, type)[1]);
		break;
	case BezierType::EaseOut:
		return mEase.Out(RecursiveLerp(mPoints, type)[0], RecursiveLerp(mPoints, type)[1]);
		break;
	default:
		return Vec3::zero;
		break;
	}
}

vector<Vec3> BezierCurve::RecursiveLerp(const vector<Vec3>& points, const BezierType type)
{
	vector<Vec3> tempPoints;

	if (points.size() < 3)
	{
		return points;
	}
	else
	{
		switch (type)
		{
		case BezierType::Lerp:
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				tempPoints.push_back(mEase.Lerp(points[i], points[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		case BezierType::EaseIn:
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				tempPoints.push_back(mEase.In(points[i], points[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		case BezierType::EaseOut:
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				tempPoints.push_back(mEase.Out(points[i], points[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		default:
			return vector<Vec3>{};
			break;
		}
	}
}

// 制御点関連
void BezierCurve::AddPoint(const Vec3& pos) { mPoints.push_back(pos); }
void BezierCurve::ClearPoints() { mPoints.clear(); }

// セッター
void BezierCurve::SetEaseTime(const int32_t easeTime) { mEase.SetEaseTimer(easeTime); }
void BezierCurve::SetEasePowNum(const float pownum) { mEase.SetPowNum(pownum); }

// ゲッター
bool BezierCurve::GetisEnd() { return mIsEnd; }