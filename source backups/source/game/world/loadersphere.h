#ifndef LOADER_SPHERE_H
#define LOADER_SPHERE_H

#include "..\..\system\typedefs.h"
#include "constants.h"

//#include "chunkoverlay.h"
#include "chunk.h"
#include "space.h"

namespace Wanderlust
{
	struct loadersphere //A thing that keeps chunks loaded for some class or another.
	{
		float Radius; //Radius in chunks.
		float X, Y, Z; //Center position in chunks.
	};
}

#endif