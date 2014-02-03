#ifndef CHUNK_OVERLAY_MANAGER_H
#define CHUNK_OVERLAY_MANAGER_H

#include "..\..\system\typedefs.h"
#include "chunkoverlaymanager.h"

namespace Wanderlust
{
	class chunkoverlaymanager //Should be inherited, never used directly.
	{
	public:
		chunkoverlaymanager(){};
		~chunkoverlaymanager(){};

		virtual void NotifyOverlayUnloaded(chunkoverlay* RemovedOverlay, int x, int y, int z){};

	protected:
		bool mEmptyOverlay;
		chunk* mOurChunk;
		chunkoverlaymanager* mManager;
	};
}

#endif