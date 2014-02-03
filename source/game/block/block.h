#ifndef BLOCK_H
#define BLOCK_H

#include "..\world\constants.h"

namespace Wanderlust
{
	class blockart;
	class block
	{
	public:
		block();
		~block();
		virtual blockart* getBlockArt();
		virtual void setBlockArt(blockart*);
	protected:
		blockart* mBlockArt;
	};
}

#endif