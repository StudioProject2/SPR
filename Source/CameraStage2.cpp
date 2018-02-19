#include "CameraStage2.h"
#include "Application.h"
#include "Mtx44.h"
#include "Box.h"

#define FLOOR_POSITION 10

CameraStage2::CameraStage2()
{
}

CameraStage2::~CameraStage2()
{
}

void CameraStage2::InteractionCheck()
{

}

void CameraStage2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	theta = 0;

	//Mouse FPS Variables
	firstrun = true;
	xyawnew = 0.0;
	yyawnew = 0.0;
	xmousepos = 0.0;
	ymousepos = 0.0;
	oldymousepos = 0.0;

	isJumpingUp = false;
	isJumpingDown = false;
	isDropping = false;
	jumpOffset = 1;
	jumpPos = 0;
	jumpSpeed = 0.0f;
	movementSpeed = 70.0f;
}

bool isPointXInBox2(Vector3 position, Box box)
{
	if (((position.x >= box.minX - 1 && position.x <= box.minX + 1)
		&& (position.y >= box.minY && position.y <= box.maxY)
		&& (position.z >= box.minZ && position.z <= box.maxZ))
		|| ((position.x >= box.maxX - 1 && position.x <= box.maxX + 1)
			&& (position.y >= box.minY && position.y <= box.maxY)
			&& (position.z >= box.minZ && position.z <= box.maxZ)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isPointYInBox2(Vector3 position, Box box)
{
	if (((position.x >= box.minX && position.x <= box.maxX)
		&& (position.y >= box.minY - 1 && position.y <= box.minY + 1)
		&& (position.z >= box.minZ && position.z <= box.maxZ))
		|| ((position.x >= box.minX && position.x <= box.maxX)
			&& (position.y >= box.maxY - 1 && position.y <= box.maxY + 1)
			&& (position.z >= box.minZ && position.z <= box.maxZ)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isPointZInBox2(Vector3 position, Box box)
{
	if (((position.x >= box.minX && position.x <= box.maxX)
		&& (position.y >= box.minY && position.y <= box.maxY)
		&& (position.z >= box.minZ - 1 && position.z <= box.minZ + 1))
		|| ((position.x >= box.minX && position.x <= box.maxX)
			&& (position.y >= box.minY && position.y <= box.maxY)
			&& (position.z >= box.maxZ - 1 && position.z <= box.maxZ + 1)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CameraStage2::BoundsCheck()
{
	//Collision
	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	Vector3 view = (target - position).Normalized();

	if (_collidedX)
	{
		position.x = prevPosX;
		target = position + view;
	}

	else if (_collidedZ)
	{
		position.z = prevPosZ;
		target = position + view;
	}
}

void CameraStage2::JumpUp(double dt)
{
	Vector3 view = (target - position).Normalized();

	if (position.y < jumpPos + 12)
	{
		position.y = position.y + (jumpSpeed * dt);
		target = position + view;
		jumpSpeed -= jumpOffset;
	}
	else
	{
		jumpSpeed = 10.0f;
		isJumpingUp = false;
		isJumpingDown = true;
	}
}

void CameraStage2::JumpDown(double dt)
{
	Vector3 view = (target - position).Normalized();


	if (position.y > jumpPos + 2 && !_collidedY)
	{
		position.y = position.y - (jumpSpeed * dt);
		target = position + view;
		jumpSpeed += jumpOffset;
	}
	else if (_collidedY)
	{
		jumpSpeed = 14.0f;
		isJumpingDown = false;
	}
	else
	{
		position.y = jumpPos;
		target = position + view;
		jumpSpeed = 14.0f;
		isJumpingDown = false;
	}
}

void CameraStage2::DropDown(double dt)
{
	Vector3 view = (target - position).Normalized();
	if (position.y > FLOOR_POSITION && !_collidedY)
	{
		position.y = position.y - (jumpSpeed * dt);
		target = position + view;
		jumpSpeed += jumpOffset;
	}
	else if (_collidedY)
	{
		isDropping = false;
	}
	else
	{
		position.y = FLOOR_POSITION;
		target = position + view;
		isDropping = false;
	}
}

void CameraStage2::Update(double dt)
{
	InteractionCheck();
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

	static const float CAMERA_SPEED = 50.f;

	prevPosX = position.x;
	prevPosY = position.y;
	prevPosZ = position.z;

	if (Application::IsKeyPressed('A'))
	{
		position = position - right * (float)(movementSpeed * dt);
		position.y = prevPosY;
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		position = position + right * (float)(movementSpeed * dt);
		position.y = prevPosY;
		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		position += view * (float)(movementSpeed * dt);
		position.y = prevPosY;
		target = position + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		position -= view * (float)(movementSpeed * dt);
		position.y = prevPosY;
		target = position + view;
	}

	BoundsCheck();

	if (isJumpingUp)
	{
		JumpUp(dt);
	}

	if (isJumpingDown)
	{
		JumpDown(dt);
	}

	if (isDropping)
	{
		DropDown(dt);
	}

	if (position.y > FLOOR_POSITION && !_collidedY && !isJumpingUp && !isJumpingDown)
	{
		isDropping = true;
	}

	if (Application::IsKeyPressed(VK_SPACE))
	{
		if (isJumpingUp == false && isJumpingDown == false && isDropping == false)
		{
			jumpSpeed = 48.0f;
			jumpPos = position.y;
			isJumpingUp = true;
		}
	}

	if (position.x > 790)
	{
		position.x = 790;
		target = position + view;
	}
	if (position.x < -790)
	{
		position.x = -790;
		target = position + view;
	}
	if (position.z > 790)
	{
		position.z = 790;
		target = position + view;
	}
	if (position.z < -790)
	{
		position.z = -790;
		target = position + view;
	}

	//Mouse FPS
	Application::GetMousePosition(xmousepos, ymousepos);

	xmousepos = xmousepos - 400.0;
	ymousepos = ymousepos - 300.0;
	double xyaw = (double)((-(xmousepos - oldxmousepos)) * 0.05);
	double yyaw = (double)((-(ymousepos - oldymousepos)) * 0.05);

	if (firstrun == false)
	{
		xyawnew = xyawnew + xyaw;
		yyawnew = yyawnew + yyaw;
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();

		Mtx44 rotation;
		rotation.SetToRotation(xyawnew, up.x, up.y, up.z);
		view = rotation * view;
		target = position + view;

		if (view.y > -0.99 && view.y < 0.7)
		{
			rotation.SetToRotation(yyawnew, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
		}
		oldxmousepos = xmousepos;
		oldymousepos = ymousepos;
	}

	firstrun = false;
	Application::SetMousePosition(400.0, 300.0);

	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}

}

void CameraStage2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
