#include "CameraDebug.h"
#include "Application.h"
#include "Mtx44.h"

CameraDebug::CameraDebug()
{
}

CameraDebug::~CameraDebug()
{
}

void CameraDebug::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	theta = 0;
}

void CameraDebug::Update(double dt)
{
	xMouse = 0;
	yMouse = 0;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	Vector3 back = right.Cross(up);

	static const float CAMERA_SPEED = 50.f;


	if (Application::IsKeyPressed('A'))
	{
		position.x = position.x - right.x * 5;
		position.z = position.z - right.z * 5;
		position.y = position.y - right.y * 5;

		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		position.x = position.x + right.x * 5;
		position.z = position.z + right.z * 5;
		position.y = position.y + right.y * 5;

		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		position.x = position.x - back.x * 5;
		position.z = position.z - back.z * 5;
		position.y = position.y - back.y * 5;
		target = position + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		position.x = position.x + back.x * 5;
		position.z = position.z + back.z * 5;
		position.y = position.y + back.y * 5;
		target = position + view;
	}

	if (Application::IsKeyPressed(VK_LEFT))
	{
		float rotateAngle = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(rotateAngle, up.x, up.y, up.z);

		view = rotation * view;
		target = position + view;

	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float rotateAngle = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(-rotateAngle, up.x, up.y, up.z);

		view = rotation * view;
		target = position + view;

	}
	if (Application::IsKeyPressed(VK_UP))
	{
		float rotateAngle = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(rotateAngle, right.x, right.y, right.z);

		view = rotation * view;
		target = position + view;

	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		float rotateAngle = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(-rotateAngle, right.x, right.y, right.z);

		view = rotation * view;
		target = position + view;
	}

	if (Application::IsKeyPressed('N'))
	{
		Vector3 direction = target - position;
		if (direction.Length() > 5)
		{
			Vector3 view = (target - position).Normalized();
			position += view * (float)(10.f * dt);
		}
	}
	if (Application::IsKeyPressed('M'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(10.f * dt);
	}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void CameraDebug::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}