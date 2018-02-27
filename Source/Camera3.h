#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "irrKlang.h"

#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;

class Camera3 : public Camera
{
private:
	bool isJumpingUp;
	bool isJumpingDown;
	bool isDropping;
	float jumpOffset;
	float jumpSpeed;
	float jumpPos;

	bool _collidedX;
	bool _collidedY;
	bool _collidedZ;

	float movementSpeed;

	void JumpUp(double dt);
	void JumpDown(double dt);
	void DropDown(double dt);

	void BoundsCheck();
	void BoundsCheckStage1();
	void BoundsCheckStage2();
	void BoundsCheckStage3();
	void BoundsCheckStage4();

	double elaspeTime;
	double footstepsTime;
public:
	float prevPosX;
	float prevPosY;
	float prevPosZ;
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
	float xyawnew;
	float yyawnew;
	double xmousepos;
	double ymousepos;
	float oldxmousepos;
	float oldymousepos;

	Camera3();
	~Camera3();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	void InteractionCheck();

	ISoundEngine* engine = createIrrKlangDevice();
};

#endif