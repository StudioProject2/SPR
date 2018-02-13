#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include "Box.h"
#include "Var.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

float DegreeToRadian3(float value)
{
	return value * 3.142 / 180.0f;
};

bool isPointInBox(Vector3 position, Box box)
{
	return (position.x >= box.minX&&position.x <= box.maxX) &&
		(position.y >= box.minY&&position.y <= box.maxY) &&
		(position.z >= box.minZ && position.z <= box.maxZ);
}

void Camera3::BoundsCheck(int direction)
{
	//Collision
	bool _collided = false;

	Box cube = Box(Vector3(0, 0, 0), 100);

	if (isPointInBox(position, cube))
	{
		_collided = true;
	}

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	Vector3 back = right.Cross(up);

	if (_collided)
	{
		std::cout << "Collision" << std::endl;

		if (direction == LEFT)
		{
			position.x = position.x + right.x * 5;
			position.z = position.z + right.z * 5;
			target = position + view;
		}
		if (direction == RIGHT)
		{
			position.x = position.x - right.x * 5;
			position.z = position.z - right.z * 5;
			target = position + view;
		}
		if (direction == UP)
		{
			position.x = position.x + back.x * 5;
			position.z = position.z + back.z * 5;
			target = position + view;
		}
		if (direction == DOWN)
		{
			position.x = position.x - back.x * 5;
			position.z = position.z - back.z * 5;
			target = position + view;
		}
	}
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


}

void Camera3::Update(double dt)
{
	InteractionCheck();
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	Vector3 back = right.Cross(up);

	static const float CAMERA_SPEED = 50.f;


	if (Application::IsKeyPressed('A'))
	{
		position.x = position.x - right.x * 5;
		position.z = position.z - right.z * 5;
		target = position + view;
		BoundsCheck(LEFT);
	}
	if (Application::IsKeyPressed('D'))
	{
		position.x = position.x + right.x * 5;
		position.z = position.z + right.z * 5;
		target = position + view;
		BoundsCheck(RIGHT);
	}
	if (Application::IsKeyPressed('W'))
	{
		position.x = position.x - back.x * 5;
		position.z = position.z - back.z * 5;
		target = position + view;
		BoundsCheck(UP);
	}
	if (Application::IsKeyPressed('S'))
	{
		position.x = position.x + back.x * 5;
		position.z = position.z + back.z * 5;
		target = position + view;
		BoundsCheck(DOWN);
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

		rotation.SetToRotation(yyawnew, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		oldxmousepos = xmousepos;
		oldymousepos = ymousepos;
	}

	firstrun = false;
	Application::SetMousePosition(400.0, 300.0);

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

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}