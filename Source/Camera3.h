#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;
	enum direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	float theta;

	//Mouse FPS Variables
	bool firstrun;
	double xyawnew;
	double yyawnew;
	double xmousepos;
	double ymousepos;
	double oldxmousepos;
	double oldymousepos;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	void BoundsCheck(int);
	void InteractionCheck();


};

#endif