#ifndef CHUNK_BLOCK_MODEL_H
#define CHUNK_BLOCK_MODEL_H

#include <gl/glew.h>
#include <gl/gl.h>

#include <SFML/OpenGL.hpp>

#include <vector>

#include "..\..\game\world\chunk.h"
#include "..\..\system\math\vector3us.h"

namespace Wanderlust
{
	class chunkblockmodel //Includes the standard blocks (that is, no slopes, no models) only.
	{
	public:
		chunkblockmodel();
		~chunkblockmodel();
		
		void regChunk(chunk*);
		
		void Rebuild();
		void Draw();

	protected:
		GLuint mVertexArrayID[6]; //One for each side.
		GLuint mVertexBufferID[6]; //One for each side.

		std::vector<vector3us> mFacesToDraw[6]; //One std::vector of all blocks to render per face (odd, I know).
		//Also, vector of vector3s. This will get confusing.
		
		chunk* mOurChunk;
		unsigned short* mOurChunkData; //Direct pointer to chunk's mChunkData.
		unsigned short* mTempBlock;
		
		unsigned short mNumBlocksDrawn;
		unsigned short mNumFacesDrawn;
		unsigned short mNumFacesDrawnPer[6];//For each side.

		GLfloat* mBufferData[6];
		//GLfloat* mSides; //Side vertex data.
		GLuint* mTextureIDBuffer[6]; //ID given as layer through texture buffer 
		GLuint mTextureIDBufferID[6]; //Brought to you by the department of redundancy department. 
		GLfloat* mUVBuffer[6]; //UV mapping squares is not difficult. :B 
		GLuint mUVBufferID[6];
	};
}

#endif