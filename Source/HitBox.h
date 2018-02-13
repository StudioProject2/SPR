#ifndef HIT_BOX_H
#define HIT_BOX_H

#include "Vector3.h"

struct HitBox
{
	Vector3 position;

	float minX;
	float maxX;
	float minZ;
	float maxZ;

	HitBox(Vector3 _position, float xSize, float zSize)
	{
		_position = position;
		minX = position.x - xSize;
		maxX = position.x + xSize;
		minZ = position.z - zSize;
		maxZ = position.z + zSize;
	}
};
#endif