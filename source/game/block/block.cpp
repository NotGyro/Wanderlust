#include "block.h"

namespace Wanderlust
{
	block::block()
	{
		mBlockArt = 0;
	}
	block::~block()
	{
	}
	blockart* block::getBlockArt()
	{
		return mBlockArt;
	}
	void block::setBlockArt(blockart* SetTo)
	{
		mBlockArt = SetTo;
	}
}