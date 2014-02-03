#include "vector3us.h"

namespace Wanderlust
{
	unsigned short vector3us::getX()
	{
		return mX;
	}
	unsigned short vector3us::getY()
	{
		return mY;
	}
	unsigned short vector3us::getZ()
	{
		return mZ;
	}
	unsigned short* vector3us::getXPtr()
	{
		return &mX;
	}
	unsigned short* vector3us::getYPtr()
	{
		return &mY;
	}
	unsigned short* vector3us::getZPtr()
	{
		return &mZ;
	}
	bool vector3us::operator == (vector3us& CompareTo)
	{
		return ((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ());
	}
	bool vector3us::operator != (vector3us& CompareTo)
	{
		return !(((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ()));
	}
	vector3us::vector3us()
	{
		mX = 0;
		mY = 0;
		mZ = 0;
	}
	vector3us::vector3us(unsigned short x, unsigned short y, unsigned short z)
	{
		mX = x;
		mY = y;
		mZ = z;
	}
}