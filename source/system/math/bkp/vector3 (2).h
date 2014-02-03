#ifndef VECTOR_3_H
#define VECTOR_3_H

namespace Wanderlust
{
	template<typename Type>
	class vector3
	{
	public: //Dot product, cross product, other wonderful stuff will come later.Type vector3::getX()
		Type vector3::getX()
		{
			return mX;
		};
		Type vector3::getY()
		{
			return mY;
		};
		Type vector3::getZ()
		{
			return mZ;
		};
		Type* vector3::getXPtr()
		{
			return &mX;
		};
		Type* vector3::getYPtr()
		{
			return &mY;
		};
		Type* vector3::getZPtr()
		{
			return &mZ;
		};
		Type vector3::operator == (vector3& CompareTo)
		{
			return ((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ());
		};
		Type vector3::operator != (vector3& CompareTo)
		{
			return !(((getX() == CompareTo.getX()) && (getY() == CompareTo.getY())) && (getZ() == CompareTo.getZ()));
		};
		vector3::vector3()
		{
			mX = 0;
			mY = 0;
			mZ = 0;
		};
		vector3::vector3(Type x, Type y, Type z)
		{
			mX = x;
			mY = y;
			mZ = z;
		};
	protected:
		union
		{
			struct
			{
				Type mX, mY, mZ;
			};
			Type mPosition[3];
		};
	};
}
#endif