#ifndef CHUNK_GENERATOR_H
#define CHUNK_GENERATOR_H

#include "..\..\..\system\typedefs.h"
#include "..\constants.h"

//#include "chunkoverlay.h"
#include "..\chunk.h"
#include "..\..\block\blockindex.h"

namespace Wanderlust
{
	class chunkgenerator
	{
	public:
		//virtual void setSeed(unsigned int);
		//virtual void setHiddenSeed(unsigned int); //The seed we don't want clients to have.
		//virtual void getSeed(unsigned int);
		//virtual void getHiddenSeed(unsigned int);
		virtual bool GenerateChunk(chunk*);
		virtual void RegBlockIndex(blockindex*);
	protected:
		blockindex* mBlockIndex;
		unsigned int mSeed;
	};
}

#endif