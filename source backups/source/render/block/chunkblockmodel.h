#ifndef CHUNK_BLOCK_MODEL_H
#define CHUNK_BLOCK_MODEL_H

#include <gl/glew.h>
#include <gl/gl.h>

#include <SFML/OpenGL.hpp>

#include <vector>

#include "..\..\game\world\chunk.h"
#include "..\..\system\math\vector3us.h"
#include "..\..\game\block\blockindex.h"

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

		void regBlockIndex(blockindex*);
	protected:
		GLuint mVertexArrayID[6]; //One for each side.
		GLuint mVertexBufferID[6]; //One for each side.
		
		struct facedrawinfo
		{
			facedrawinfo(blockart* BA, vector3us XYZ)
			{
				Block = BA;
				Position = XYZ;
			}
			blockart* Block;
			vector3us Position;
		};
		std::vector<facedrawinfo> mFacesToDraw[6]; //One std::vector of all blocks to render per face (odd, I know).
		//Also, vector of vector3s. This will get confusing.
		
		chunk* mOurChunk;
		blockindex* mBlockIndex;
		blockID* mTempBlock;
		
		unsigned short mNumBlocksDrawn;
		unsigned short mNumFacesDrawn;
		unsigned short mNumFacesDrawnPer[6];//For each side.

		GLfloat* mBufferData[6];
		//GLfloat* mSides; //Side vertex data.
		GLuint* mTextureIDBuffer[6]; //ID given as layer through texture buffer 
		GLuint mTextureIDBufferID[6]; //Brought to you by the department of redundancy department. 
		GLfloat* mUVBuffer[6]; //UV mapping squares is not difficult. :B 
		GLuint mUVBufferID[6];

		inline bool checkcullside(blockart*, axis, unsigned short, unsigned short, unsigned short); //Pretty much just exists to make Rebuild() prettier.
		struct sidecullinfo
		{
			blockart* Block;
			bool Culled;
		};
		sidecullinfo mSideCulling[6][CHUNK_SIZE * CHUNK_SIZE];
	};
}

#endif