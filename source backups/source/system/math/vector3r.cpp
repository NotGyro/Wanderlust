#include "vector3r.h"

namespace Wanderlust
{
	
	real vector3r::getX()
	{
		return mX;
	}
	real vector3r::getY()
	{
		return mY;
	}
	real vector3r::getZ()
	{
		return mZ;
	}
	real* vector3r::getXPtr()
	{
		return &mX;
	}
	real* vector3r::getYPtr()
	{
		return &mY;
	}
	real* vector3r::getZPtr()
	{
		return &mZ;
	}
	bool vector3r::operator == (vector3r& CompareTo)
	{
		return ((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ());
	}
	bool vector3r::operator != (vector3r& CompareTo)
	{
		return !(((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ()));
	}
	vector3r::vector3r()
	{
		mX = 0;
		mY = 0;
		mZ = 0;
	}
	vector3r::vector3r(real x, real y, real z)
	{
		mX = x;
		mY = y;
		mZ = z;
	}
}