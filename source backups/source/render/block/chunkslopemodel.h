#include <gl/glew.h>
#include <gl/gl.h>
#ifndef CHUNK_BLOCK_MODEL_H
#define CHUNK_BLOCK_MODEL_H

#include <SFML/OpenGL.hpp>
#include "..\..\game\world\chunk.h"

namespace Wanderlust
{
	class chunkblockmodel //Includes the standard blocks (that is, no slopes, no models) only.
	{
	public:
		chunkblockmodel();
		~chunkblockmodel();
		
		void regChunk(chunk*);
		
		void Rebuild(); //Reload the OpenGL model
		void CleanRebuild(); //Rebuild the faces from scratch 
		void NotifyChange(unsigned short, unsigned short, unsigned short);

	protected:
		GLuint mVertexArrayID[6]; //One for each side.
		GLuint mVertexBufferID[6]; //One for each side.
		
		chunk* mOurChunk;
		unsigned short* mOurChunkData; //Direct pointer to chunk's mChunkData.

		bool mFacesExposed[((CHUNK_SIZE * CHUNK_SIZE) * CHUNK_SIZE)*6];
	};
}

#endif