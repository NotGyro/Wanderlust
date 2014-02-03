#ifndef BLOCKDATA_H
#define BLOCKDATA_H
#include "..\..\core\typedefs.h"

namespace Wanderlust
{
	struct blockdata
	{
		unsigned short int BlockID; //Saved and loaded as 12 bits, range 0 to 4095 (4096 possible values).
		byte MetaData; //Saved and loaded as 4 bits, range 0 to 15 (16 possible values).
	};
}

#endif