#ifndef CAMERA_DEBUG
#define CAMERA_DEBUG

#include "Camera.h"

class CameraDebug : public Camera
{
public:


	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	double xMouse;
	double yMouse;
	float theta;

	CameraDebug();
	~CameraDebug();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif