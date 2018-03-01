#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include "Box.h"

#define FLOOR_POSITION 10

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::InteractionCheck()
{

}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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
	movementSpeed = 100.0f;

	elaspeTime = 0.0;
	footstepsTime = 0.0;
}

bool isPointXInBox(Vector3 position, Box box)
{
	if (((position.x >= box.minX - 2 && position.x <= box.minX + 2)
		&& (position.y >= box.minY && position.y <= box.maxY)
		&& (position.z >= box.minZ && position.z <= box.maxZ))
		|| ((position.x >= box.maxX - 2 && position.x <= box.maxX + 2)
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

bool isPointYInBox(Vector3 position, Box box)
{
	if (((position.x >= box.minX && position.x <= box.maxX)
		&& (position.y >= box.minY - 2 && position.y <= box.minY + 2)
		&& (position.z >= box.minZ && position.z <= box.maxZ))
		|| ((position.x >= box.minX && position.x <= box.maxX)
			&& (position.y >= box.maxY - 2 && position.y <= box.maxY + 2)
			&& (position.z >= box.minZ && position.z <= box.maxZ)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isPointZInBox(Vector3 position, Box box)
{
	if (((position.x >= box.minX && position.x <= box.maxX)
		&& (position.y >= box.minY && position.y <= box.maxY)
		&& (position.z >= box.minZ - 2 && position.z <= box.minZ + 2))
		|| ((position.x >= box.minX && position.x <= box.maxX)
			&& (position.y >= box.minY && position.y <= box.maxY)
			&& (position.z >= box.maxZ - 2 && position.z <= box.maxZ + 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Camera3::BoundsCheck()
{
	//Collision
	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	Box cube = Box(Vector3(0, 0, 0), 105);
	Box cube2 = Box(Vector3(100, 0, 150), 3.5);

	if (isPointXInBox(position, cube)
		|| isPointXInBox(position, cube2)
		)
	{
		_collidedX = true;
	}

	if (isPointYInBox(position, cube)
		|| isPointYInBox(position, cube2)
		)
	{
		_collidedY = true;
	}

	if (isPointZInBox(position, cube)
		|| isPointZInBox(position, cube2)
		)
	{
		_collidedZ = true;
	}

	Vector3 view = (target - position).Normalized();

	if (_collidedX)
	{
		position.x = prevPosX;
		target = position + view;
	}

	if (_collidedZ)
	{
		position.z = prevPosZ;
		target = position + view;
	}
}

void Camera3::BoundsCheckStage1()
{
	//Collision
	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	Box cage = Box(Vector3(500, 0, 575), 70, 7, 70);
	Box cage2 = Box(Vector3(500, 0, 430), 70, 7, 70);
	Box cage3 = Box(Vector3(570, 0, 502.5), 7, 70, 70);

	Box cube2 = Box(Vector3(100, 0, 150), 3.5);

	if (isPointXInBox(position, cage) 
		|| isPointXInBox(position, cage2) 
		|| isPointXInBox(position, cage3)
		|| isPointXInBox(position, cube2)
		)
	{
		_collidedX = true;
	}

	if (isPointYInBox(position, cage) 
		|| isPointYInBox(position, cage2) 
		|| isPointYInBox(position, cage3)
		|| isPointYInBox(position, cube2)
		)
	{
		_collidedY = true;
	}

	if (isPointZInBox(position, cage)
		|| isPointZInBox(position, cage2)
		|| isPointZInBox(position, cage3)
		|| isPointZInBox(position, cube2)
		)
	{
		_collidedZ = true;
	}

	Vector3 view = (target - position).Normalized();

	if (_collidedX)
	{
		position.x = prevPosX;
		target = position + view;
	}

	if (_collidedZ)
	{
		position.z = prevPosZ;
		target = position + view;
	}

	
	if (position.x > 595)
	{
		position.x = 595;
		target = position + view;
	}
	if (position.x < -595)
	{
		position.x = -595;
		target = position + view;
	}
	if (position.z > 595)
	{
		position.z = 595;
		target = position + view;
	}
	if (position.z < -595)
	{
		position.z = -595;
		target = position + view;
	}
	
}

void Camera3::BoundsCheckStage2()
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

	if (_collidedZ)
	{
		position.z = prevPosZ;
		target = position + view;
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
}

void Camera3::BoundsCheckStage3()
{
	//Collision
	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	Box hut1 = Box(Vector3(200, -10, 0), 50);
	Box hut2 = Box(Vector3(200, -10, 200), 50);
	Box hut3 = Box(Vector3(-200, -10, 0), 50);
	Box hut4 = Box(Vector3(-200, -10, 200), 50);

	if (isPointXInBox(position, hut1)
		|| isPointXInBox(position, hut2)
		|| isPointXInBox(position, hut3)
		|| isPointXInBox(position, hut4)
		)
	{
		_collidedX = true;
	}

	if (isPointYInBox(position, hut1)
		|| isPointYInBox(position, hut2)
		|| isPointYInBox(position, hut3)
		|| isPointYInBox(position, hut4)
		)
	{
		_collidedY = true;
	}

	if (isPointZInBox(position, hut1)
		|| isPointZInBox(position, hut2)
		|| isPointZInBox(position, hut3)
		|| isPointZInBox(position, hut4)
		)
	{
		_collidedZ = true;
	}

	Vector3 view = (target - position).Normalized();

	if (_collidedX)
	{
		position.x = prevPosX;
		target = position + view;
	}

	if (_collidedZ)
	{
		position.z = prevPosZ;
		target = position + view;
	}

	if (position.x > 290)
	{
		position.x = 290;
		target = position + view;
	}
	if (position.x < -290)
	{
		position.x = -290;
		target = position + view;
	}
	if (position.z > 890)
	{
		position.z = 890;
		target = position + view;
	}
	if (position.z < -890)
	{
		position.z = -890;
		target = position + view;
	}
}

void Camera3::BoundsCheckStage4()
{
	//Collision
	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	Box hut1 = Box(Vector3(200, -10, 0), 50);
	Box hut2 = Box(Vector3(200, -10, 200), 50);
	Box hut3 = Box(Vector3(200, -10, -200), 50);
	Box hut4 = Box(Vector3(-200, -10, 0), 50);
	Box hut5 = Box(Vector3(-200, -10, 200), 50);
	Box hut6 = Box(Vector3(-200, -10, -200), 50);

	if (isPointXInBox(position, hut1)
		|| isPointXInBox(position, hut2)
		|| isPointXInBox(position, hut3)
		|| isPointXInBox(position, hut4)
		|| isPointXInBox(position, hut5)
		|| isPointXInBox(position, hut6)
		)
	{
		_collidedX = true;
	}

	if (isPointYInBox(position, hut1)
		|| isPointYInBox(position, hut2)
		|| isPointYInBox(position, hut3)
		|| isPointYInBox(position, hut4)
		|| isPointYInBox(position, hut5)
		|| isPointYInBox(position, hut6)
		)
	{
		_collidedY = true;
	}

	if (isPointZInBox(position, hut1)
		|| isPointZInBox(position, hut2)
		|| isPointZInBox(position, hut3)
		|| isPointZInBox(position, hut4)
		|| isPointZInBox(position, hut5)
		|| isPointZInBox(position, hut6)
		)
	{
		_collidedZ = true;
	}

	Vector3 view = (target - position).Normalized();

	if (_collidedX)
	{
		position.x = prevPosX;
		target = position + view;
	}

	if (_collidedZ)
	{
		position.z = prevPosZ;
		target = position + view;
	}

	if (position.x > 290)
	{
		position.x = 290;
		target = position + view;
	}
	if (position.x < -290)
	{
		position.x = -290;
		target = position + view;
	}
	if (position.z > 890)
	{
		position.z = 890;
		target = position + view;
	}
	if (position.z < -890)
	{
		position.z = -890;
		target = position + view;
	}
}

void Camera3::JumpUp(double dt)
{
	Vector3 view = (target - position).Normalized();

	if (position.y < jumpPos + 12)
	{
		position.y = position.y + (jumpSpeed * (float)dt);
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

void Camera3::JumpDown(double dt)
{
	Vector3 view = (target - position).Normalized();


	if (position.y > jumpPos + 2 && !_collidedY)
	{
		position.y = position.y - (jumpSpeed * (float)dt);
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

void Camera3::DropDown(double dt)
{
	Vector3 view = (target - position).Normalized();
	if (position.y > FLOOR_POSITION && !_collidedY)
	{
		position.y = position.y - (jumpSpeed * (float)dt);
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

void Camera3::Update(double dt)
{
	InteractionCheck();
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	elaspeTime += dt;

	static const float CAMERA_SPEED = 50.f;

	prevPosX = position.x;
	prevPosY = position.y;
	prevPosZ = position.z;

	if (Application::IsKeyPressed('A'))
	{
		if (elaspeTime > footstepsTime && position.y == FLOOR_POSITION && !Application::muted)
		{
			engine->play2D("Sound/footsteps.wav", false);
			footstepsTime = elaspeTime + 0.3;
		}
		position = position - right * (float)(movementSpeed * dt);
		position.y = prevPosY;
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		if (elaspeTime > footstepsTime && position.y == FLOOR_POSITION && !Application::muted)
		{
			engine->play2D("Sound/footsteps.wav", false);
			footstepsTime = elaspeTime + 0.3;
		}
		position = position + right * (float)(movementSpeed * dt);
		position.y = prevPosY;
		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		if (elaspeTime > footstepsTime && position.y == FLOOR_POSITION && !Application::muted)
		{
			engine->play2D("Sound/footsteps.wav", false);
			footstepsTime = elaspeTime + 0.3;
		}
		position += view * (float)(movementSpeed * dt);
		position.y = prevPosY;
		target = position + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		if (elaspeTime > footstepsTime && position.y == FLOOR_POSITION && !Application::muted)
		{
			engine->play2D("Sound/footsteps.wav", false);
			footstepsTime = elaspeTime + 0.3;
		}
		position -= view * (float)(movementSpeed * dt);
		position.y = prevPosY;
		target = position + view;
	}

	if (Application::whatScene == Application::STAGE1)
	{
		BoundsCheckStage1();
	}
	if (Application::whatScene == Application::STAGE2)
	{
		BoundsCheckStage2();
	}
	if (Application::whatScene == Application::STAGE3)
	{
		BoundsCheckStage3();
	}
	if (Application::whatScene == Application::STAGE4)
	{
		BoundsCheckStage4();
	}

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

	//Mouse FPS
	Application::GetMousePosition(xmousepos, ymousepos);

	xmousepos = xmousepos - 400.0;
	ymousepos = ymousepos - 300.0;
	float xyaw = (float)((-(xmousepos - oldxmousepos)) * 0.05);
	float yyaw = (float)((-(ymousepos - oldymousepos)) * 0.05);

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


		rotation.SetToRotation(yyawnew, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;

		oldxmousepos = (float)xmousepos;
		oldymousepos = (float)ymousepos;
	}

	firstrun = false;
	Application::SetMousePosition(400.0, 300.0);

	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}

}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
