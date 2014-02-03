#ifndef VECTOR_3_H
#define VECTOR_3_H

#include "typedefs.h"

namespace Wanderlust
{
#define vector3 vector3r
	class vector3r
	{
	public: //Dot product, cross product, other wonderful stuff will come later.
		vector3r();
		vector3r(real, real, real);
		real getX();
		real getY();
		real getZ();
		real* getXPtr();
		real* getYPtr();
		real* getZPtr();
		bool operator==(vector3r&);
		bool operator!=(vector3r&);
	protected:
		union
		{
			struct
			{
				real mX, mY, mZ;
			};
			real mPosition[3];
		};
	};
}
#endif