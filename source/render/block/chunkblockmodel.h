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
		void Rebuild(axis); //Rebuild a side only
		void Draw();

		void regBlockIndex(blockindex*);
	protected:
#define FLOATS_PER_VERTEX 3
#define FLOATS_PER_VERTEX_UVL 3
#define VERTEXES_PER_FACE 6
#define FLOATS_PER_FACE VERTEXES_PER_FACE * FLOATS_PER_VERTEX
#define FLOATS_PER_FACE_UVL VERTEXES_PER_FACE * FLOATS_PER_VERTEX_UVL

		chunk* mOurChunk;
		blockindex* mBlockIndex;
		blockID* mTempBlock;

		GLuint mSideVertexArrayID[6]; //One for each side.
		GLuint mCoreVertexArrayID;

		unsigned short mNumFacesDrawn;
		unsigned short mCoreNumFacesDrawn;
		unsigned short mSideNumFacesDrawn[6];//For each side.
		
		GLuint mSideVertexBufferID[6];
		GLfloat mSideBufferData[6][FLOATS_PER_FACE * (CHUNK_SIZE * CHUNK_SIZE)];
		GLfloat mSideUVLBuffer[6][FLOATS_PER_FACE_UVL * (CHUNK_SIZE * CHUNK_SIZE)];
		GLuint mSideUVLBufferID[6];
		
		GLuint mCoreVertexBufferID;
		GLfloat mCoreBufferData[(FLOATS_PER_FACE * 6 * ((CHUNK_SIZE * CHUNK_SIZE) * CHUNK_SIZE)) - ((FLOATS_PER_FACE * (CHUNK_SIZE * CHUNK_SIZE)) * 6)];
		GLfloat mCoreUVLBuffer[(FLOATS_PER_FACE_UVL * 6 * ((CHUNK_SIZE * CHUNK_SIZE) * CHUNK_SIZE)) - ((FLOATS_PER_FACE_UVL * (CHUNK_SIZE * CHUNK_SIZE)) * 6)];
		GLuint mCoreUVLBufferID;

		inline bool checkcullface(blockart*, axis, unsigned short, unsigned short, unsigned short); //Pretty much just exists to make Rebuild() prettier.
		inline bool isOnSide(axis, unsigned short, unsigned short, unsigned short);
	};
}

#endif