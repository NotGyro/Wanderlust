#ifndef VECTOR_3_H
#define VECTOR_3_H

namespace Wanderlust
{
#define vector3 vector3f
	class vector3f
	{
	public: //Dot product, cross product, other wonderful stuff will come later.
		vector3f();
		vector3f(float, float, float);
		float getX();
		float getY();
		float getZ();
		float* getXPtr();
		float* getYPtr();
		float* getZPtr();
		bool operator==(vector3f&);
		bool operator!=(vector3f&);
	protected:
		union
		{
			struct
			{
				float mX, mY, mZ;
			};
			float mPosition[3];
		};
	};
}
#endif