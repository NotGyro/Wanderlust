#include "vector3f.h"

namespace Wanderlust
{
	
	float vector3f::getX()
	{
		return mX;
	}
	float vector3f::getY()
	{
		return mY;
	}
	float vector3f::getZ()
	{
		return mZ;
	}
	float* vector3f::getXPtr()
	{
		return &mX;
	}
	float* vector3f::getYPtr()
	{
		return &mY;
	}
	float* vector3f::getZPtr()
	{
		return &mZ;
	}
	bool vector3f::operator == (vector3f& CompareTo)
	{
		return ((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ());
	}
	bool vector3f::operator != (vector3f& CompareTo)
	{
		return !(((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ()));
	}
	vector3f::vector3f()
	{
		mX = 0;
		mY = 0;
		mZ = 0;
	}
	vector3f::vector3f(float x, float y, float z)
	{
		mX = x;
		mY = y;
		mZ = z;
	}
}