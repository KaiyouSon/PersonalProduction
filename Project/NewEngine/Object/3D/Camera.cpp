#include "Camera.h"
#include "NewEngine.h"

Camera Camera::current = {};

Camera::Camera() :
	pos(0, 0, -30), rot(0, 0, 0), fov(Radian(45)),
	oNearZ(0), oFarZ(1), rect(0, 1920, 0, 1080),
	pNearZ(0.1f), pFarZ(10000.0f)
{
	// ���Ă�
	Update();
}

void Camera::Update()
{
	// ��]�֘A
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.GetWorldMat().GetZAxis();
	Vec3 v3 = transform.GetWorldMat().GetYAxis();

	// �r���[�ϊ��s��
	mViewLookToMat = ConvertViewProjectionMatLookTo(v1, v2, v3);
	mViewLookAtMat = ConvertViewProjectionMatLookAt(v1, v1 * 10, { 0,1,0 });

	// ���s���e�s��̌v�Z
	mOrthoGrphicProjectionMat = ConvertOrthoGrphicProjectionMat(
		GetWindowSize().x, GetWindowSize().y);

	mOrthoGrphicProjectionMat = ConvertOrthoGrphicProjectionMat(rect, oNearZ, oFarZ);

	// �������e�s��̌v�Z
	mPerspectiveProjectionMat = ConvertPerspectiveProjectionMat(
		fov, (float)GetWindowSize().x / GetWindowSize().y, pNearZ, pFarZ);
}

void Camera::DebugCameraUpdate()
{
	const Vec3 frontVec =
	{
		sinf(current.rot.y),
		-sinf(current.rot.x),
		cosf(current.rot.y),
	};

	// ��]
	if (Mouse::GetClick(MouseCode::Wheel) && !Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMoveVec().x != 0 || Mouse::GetMoveVec().y != 0)
		{
			const float moveSpeed = 0.005f;
			current.rot.x += Mouse::GetMoveVec().y * moveSpeed;
			current.rot.y += Mouse::GetMoveVec().x * moveSpeed;
		}
	}

	// ���s�ړ�
	if (Mouse::GetClick(MouseCode::Wheel) && Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMoveVec().x != 0 || Mouse::GetMoveVec().y != 0)
		{
			const Vec3 rightVec = Vec3::Cross(frontVec, { 0,1,0 });
			current.pos += rightVec * Mouse::GetMoveVec().x * 0.1f;
		}
	}

	// ���s
	if (Mouse::GetWheelMoveVec() != 0)
	{
		const float moveSpeed = 0.025f;
		current.pos += frontVec * Mouse::GetWheelMoveVec() * moveSpeed;
	}
}