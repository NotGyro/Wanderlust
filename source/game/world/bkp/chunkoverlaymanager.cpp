#include "chunkoverlay.h"
#include "chunk.h"

namespace Wanderlust
{
	chunkoverlay::chunkoverlay()
	{
		mOurChunk = 0;
	}
	chunkoverlay::~chunkoverlay()
	{

	}
	void chunkoverlay::RegisterOurChunk(chunk* RegisterThis)
	{
		mOurChunk = RegisterThis;
	}
	void chunkoverlay::RegisterManager(chunkoverlaymanager*)
	{
	}
	chunkoverlaymanager* chunkoverlay::getManager()
	{
	}
	chunk* chunkoverlay::getOurChunk()
	{
		return mOurChunk;
	}
}