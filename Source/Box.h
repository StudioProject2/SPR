#ifndef Box_H
#define Box_H

#include "Vector3.h"

struct Box
{
	Vector3 position;

	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;

	Box(Vector3 _position, float _size)
	{
		position = _position;
		minX = position.x - _size;
		maxX = position.x + _size;
		minY = position.y - _size;
		maxY = position.y + _size;
		minZ = position.z - _size;
		maxZ = position.z + _size;
	}
};
#endif
