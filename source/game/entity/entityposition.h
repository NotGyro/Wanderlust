#ifndef ENTITY_POSITION_H
#define ENTITY_POSITION_H

#include "..\system\math\typedefs.h"
#include "..\system\math\vector3r.h"

namespace Wanderlust
{
	class entityposition
	{
	public:
		entityposition();
		entityposition(real, real, real);
		entityposition(int, int, int);
		entityposition(real, real, real, int, int, int);
		int getChunkX();
		int getChunkY();
		int getChunkZ();
		int* getXPtr();
		int* getYPtr();
		int* getZPtr();
		bool operator==(vector3i&);
		bool operator!=(vector3i&);
	private:
		vector3r mRelativePosition; //Position relative to mChunkPosition.
		vector3i mChunkPosition; //Chunk whose center mRelativePosition is relative to.
	}
}

#endif