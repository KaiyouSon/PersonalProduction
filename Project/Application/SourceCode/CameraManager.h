#pragma once
#include "NewEngine.h"
#include "ICamera.h"

template<typename> class Singleton;

class CameraManager : public Singleton<CameraManager>
{
public:
	enum class CameraType
	{
		Default,
		Target,
	};

private:
	std::unique_ptr<ICamera> currentCamera_;
	CameraType cameraType_;
	Player* player_;

public:
	CameraManager();
	void Init();
	void Update();

	void ChangeCamera(const CameraType cameraType);

public:
	CameraType GetCameraType();

public:
	void SetPlayer(Player* player);

private:
	friend Singleton<CameraManager>;
};

