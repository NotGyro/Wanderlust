#ifndef VECTOR_3_US_H
#define VECTOR_3_US_H

//The US is for Unsigned Short.

namespace Wanderlust
{
	class vector3us
	{
	public: //Dot product, cross product, other wonderful stuff will come later.
		vector3us();
		vector3us(unsigned short, unsigned short, unsigned short);
		unsigned short getX();
		unsigned short getY();
		unsigned short getZ();
		unsigned short* getXPtr();
		unsigned short* getYPtr();
		unsigned short* getZPtr();
		bool operator==(vector3us&);
		bool operator!=(vector3us&);
	protected:
		union
		{
			struct
			{
				unsigned short mX, mY, mZ;
			};
			unsigned short mPosition[3];
		};
	};
}
#endif