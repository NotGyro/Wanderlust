#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "math\vector3r.h"

namespace Wanderlust
{
	typedef vector3 entityposition;
	typedef unsigned char byte;
	enum axis
	{
		POSITIVE_X,
		NEGATIVE_X,
		POSITIVE_Y,
		NEGATIVE_Y,
		POSITIVE_Z,
		NEGATIVE_Z
	};

	static const axis NumToAxis[6] =
	{
		POSITIVE_X,
		NEGATIVE_X,
		POSITIVE_Y,
		NEGATIVE_Y,
		POSITIVE_Z,
		NEGATIVE_Z
	};
}
#endif