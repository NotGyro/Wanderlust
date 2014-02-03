#ifndef VECTOR_3_H
#define VECTOR_3_H


namespace Wanderlust
{
	class vector3i
	{
	public: //Dot product, cross product, other wonderful stuff will come later.
		vector3i();
		vector3i(int, int, int);
		int getX();
		int getY();
		int getZ();
		int* getXPtr();
		int* getYPtr();
		int* getZPtr();
		bool operator==(vector3i&);
		bool operator!=(vector3i&);
	protected:
		union
		{
			struct
			{
				int mX, mY, mZ;
			};
			int mPosition[3];
		};
	};
}
#endif