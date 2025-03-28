#pragma once
#include "NewEngine.h"
#include "ICamera.h"
#include "Player.h"
#include "Boss.h"

// 前方宣言
template<typename> class Singleton;

// カメラを管理するクラス
class CameraManager : public Singleton<CameraManager>
{
public:
	enum CameraType
	{
		Default,
		Target,
		Movie,
	};

private:
	std::unique_ptr<ICamera> mCurrentCamera;
	CameraType mCameraType;
	Player* mPlayer;
	Boss* mBoss;

public:
	CameraManager();
	void Init();
	void Update();

	void ChangeCamera(const CameraType cameraType);

public:
	CameraType GetCameraType();

public:
	void SetPlayer(Player* player);
	void SetBoss(Boss* boss);

private:
	friend Singleton<CameraManager>;
};

