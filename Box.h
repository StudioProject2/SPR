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

	Box(Vector3 _position, float sizeX, float sizeZ)
	{
		position = _position;
		minX = position.x - sizeX;
		maxX = position.x + sizeX;
		minY = position.y - 1000;
		maxY = position.y + 1000;
		minZ = position.z - sizeZ;
		maxZ = position.z + sizeZ;
	}
	Box(Vector3 _position, float sizeX, float sizeZ, float sizeY)
	{
		position = _position;
		minX = position.x - sizeX;
		maxX = position.x + sizeX;
		minY = position.y - sizeY;
		maxY = position.y + sizeY;
		minZ = position.z - sizeZ;
		maxZ = position.z + sizeZ;
	}
};
#endif
