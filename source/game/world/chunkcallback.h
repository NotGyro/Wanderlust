#ifndef CHUNK_LOAD_CALLBACKS
#define CHUNK_LOAD_CALLBACKS

#include "chunk.h"

namespace Wanderlust
{
	struct chunkcallback
	{
		virtual void Callback(chunk*){};
	};
}
#endif