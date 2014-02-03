#include "vector3i.h"

namespace Wanderlust
{
	
	int vector3i::getX()
	{
		return mX;
	}
	int vector3i::getY()
	{
		return mY;
	}
	int vector3i::getZ()
	{
		return mZ;
	}
	int* vector3i::getXPtr()
	{
		return &mX;
	}
	int* vector3i::getYPtr()
	{
		return &mY;
	}
	int* vector3i::getZPtr()
	{
		return &mZ;
	}
	bool vector3i::operator == (vector3i& CompareTo)
	{
		return ((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ());
	}
	bool vector3i::operator != (vector3i& CompareTo)
	{
		return !(((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ()));
	}
	vector3i::vector3i()
	{
		mX = 0;
		mY = 0;
		mZ = 0;
	}
	vector3i::vector3i(int x, int y, int z)
	{
		mX = x;
		mY = y;
		mZ = z;
	}
}