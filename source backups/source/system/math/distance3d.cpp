#include "distance3d.h"
#include <math.h>

namespace Wanderlust
{
	float Distance3D(vector3r Start, vector3r End)
	{
		return sqrt(pow(Start.getX() - End.getX(), 2) + pow(Start.getY() - End.getY(), 2) + pow(Start.getZ() - End.getZ(), 2));
	}
}