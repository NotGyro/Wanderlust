#ifndef BLOCK_LAYER_ABSTRACT_H
#define BLOCK_LAYER_ABSTRACT_H

#include "..\..\..\system\typedefs.h"

#include "..\chunklayer.h"

namespace Wanderlust
{
	class blocklayer : public chunklayer
	{
	public:
		virtual byte getBlock(unsigned short, unsigned short, unsigned short){return 0;}; //X, Y, Z
		virtual void setBlock(unsigned short, unsigned short, unsigned short, byte){}; //X, Y, Z, data.
	};
}

#endif