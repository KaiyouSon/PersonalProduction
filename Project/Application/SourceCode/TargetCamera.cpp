#include "TargetCamera.h"

TargetCamera::TargetCamera()
{
}

void TargetCamera::Init(Player* player)
{
	player_ = player;
	isEaseEnd = false;


	//const float length = 30.f;
	//Vec3 target = Vec3(0.f, 7.125f, 20.f);
	//Vec3 curPos = player_->GetPos();
	//curPos.y = 9.5f;
	//Vec3 vec = curPos - target;
	//vec.y = 0;
	//targetPos_ = curPos + vec.Norm() * length;

	//Vec3 disToCamera = target - targetPos_;
	//float pitch = atan2f(disToCamera.y, disToCamera.z);
	//float yawcRad = atan2f(disToCamera.x, disToCamera.z);
	//// 回転の処理
	//Vec3 rot =
	//{
	//	//pitch,
	//	0.f,
	//	yawcRad,
	//	0.f,
	//};
	//// 角度の設定
	//targetRot_ = rot;
}

void TargetCamera::Update()
{
	Vec3 target = Vec3(0.f, 7.125f, 20.f);

	// 座標の設定

	const float length = 30.f;
	Vec3 curPos = player_->GetPos();
	curPos.y = 9.5f;
	Vec3 vec = curPos - target;
	vec.y = 0;
	targetCamePos_ = curPos + vec.Norm() * length;
	curCamePos_ += (targetCamePos_ - camera_->pos) * 0.25f;
	camera_->pos = curCamePos_;

	Vec3 disToCamera = target - curPos;
	float pitch = atan2f(disToCamera.y, disToCamera.z);
	float yawcRad = atan2f(disToCamera.x, disToCamera.z);

	// 回転の処理
	Vec3 rot =
	{
		//pitch,
		0.f,
		yawcRad,
		0.f,
	};
	// 角度の設定
	camera_->rot = rot;

	camera_->pos = Camera::current.pos;
	camera_->rot = Camera::current.rot;
	targetPos_ = targetCamePos_;
	targetRot_ = rot;
	EaseCamera();

	// 現在いのカメラに代入
	Camera::current = *camera_;
}
