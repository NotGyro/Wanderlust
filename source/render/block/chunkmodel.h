#include <gl/glew.h>
#include <gl/gl.h>
#ifndef CHUNK_BLOCK_MODEL_H
#define CHUNK_BLOCK_MODEL_H

#include <SFML/OpenGL.hpp>
#include "..\..\game\world\chunk.h"

namespace Wanderlust
{
	class chunkmodel //Includes the standard blocks (that is, no slopes, no models) only.
	{
	public:
		chunkmodel();
		~chunkmodel();
		
		void regChunk(chunk*);
		
		void Rebuild();

	protected:
	};
}

#endif