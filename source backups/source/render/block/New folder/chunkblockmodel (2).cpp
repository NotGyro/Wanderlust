#include "chunkblockmodel.h"
#include <gl/glew.h>
#include <gl/gl.h>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "..\..\system\typedefs.h"
#include "..\..\system\math\vector3f.h"

namespace Wanderlust
{
	//Got to use indexed vertexes for each face later.
	
#define POSX_POSY_POSZ_VERT 1,1,1
#define POSX_POSY_NEGZ_VERT 1,1,0
#define POSX_NEGY_NEGZ_VERT 1,0,0
#define POSX_NEGY_POSZ_VERT 1,0,1
#define NEGX_POSY_NEGZ_VERT 0,1,0
#define NEGX_POSY_POSZ_VERT 0,1,1
#define NEGX_NEGY_POSZ_VERT 0,0,1
#define NEGX_NEGY_NEGZ_VERT 0,0,0
	
	static const GLfloat gSides[] =
	{
		//corresponds to Wanderlust::direction

		//Need to make these into actual triangles instead of quadrangles.
		//POSITIVE_X:
		//-First triangle:
		POSX_POSY_POSZ_VERT,
		POSX_NEGY_POSZ_VERT,
		POSX_NEGY_NEGZ_VERT,
		//-Second triangle:
		POSX_POSY_NEGZ_VERT,
		POSX_POSY_POSZ_VERT,
		POSX_NEGY_NEGZ_VERT,

		//NEGATIVE_X
		//-First triangle:
		NEGX_NEGY_NEGZ_VERT,
		NEGX_NEGY_POSZ_VERT,
		NEGX_POSY_POSZ_VERT,
		//-Second triangle
		NEGX_NEGY_NEGZ_VERT,
		NEGX_POSY_POSZ_VERT,
		NEGX_POSY_NEGZ_VERT,

		//POSITIVE_Y
		//-First triangle:
		POSX_POSY_POSZ_VERT,
		POSX_POSY_NEGZ_VERT,
		NEGX_POSY_NEGZ_VERT,
		//-Second triangle
		NEGX_POSY_POSZ_VERT,
		POSX_POSY_POSZ_VERT,
		NEGX_POSY_NEGZ_VERT,
		
		//NEGATIVE_Y
		//-First triangle:
		NEGX_NEGY_NEGZ_VERT,
		POSX_NEGY_NEGZ_VERT,
		POSX_NEGY_POSZ_VERT,
		//-Second triangle
		NEGX_NEGY_NEGZ_VERT,
		POSX_NEGY_POSZ_VERT,
		NEGX_NEGY_POSZ_VERT,

		//POSITIVE_Z
		//-First triangle:
		NEGX_NEGY_POSZ_VERT,
		POSX_NEGY_POSZ_VERT,
		POSX_POSY_POSZ_VERT,
		//-Second triangle
		NEGX_NEGY_POSZ_VERT,
		POSX_POSY_POSZ_VERT,
		NEGX_POSY_POSZ_VERT,

		//NEGATIVE_Z
		//-First triangle:
		POSX_POSY_NEGZ_VERT,
		POSX_NEGY_NEGZ_VERT,
		NEGX_NEGY_NEGZ_VERT,
		//-Second triangle
		NEGX_POSY_NEGZ_VERT,
		POSX_POSY_NEGZ_VERT,
		NEGX_NEGY_NEGZ_VERT,
	};

#define FLOATS_PER_FACE 18

	chunkblockmodel::chunkblockmodel()
	{
		for(unsigned short i = 0; i < 6; i++)
		{
			mBufferData[i] = 0;
			mVertexBufferID[i] = 0;
			mVertexArrayID[i] = 0;
			mNumFacesDrawnPer[i] = 0;
		}
		mOurChunk = 0;
		mNumFacesDrawn = 0;
		//mSides = new GLfloat[((6/*sides*/ * 2 /*triangles*/)*4/*vertecies*/)*3/*values per vertex*/];
	}
	chunkblockmodel::~chunkblockmodel()
	{
		if(mNumFacesDrawn != 0)
		{
			for(unsigned short i = 0; i < 6; i++)
			{
				delete [] mBufferData[i];
				mBufferData[i] = 0;
				mFacesToDraw[i].clear();
			}
			glDeleteBuffers(6, mVertexBufferID);
			glDeleteVertexArrays(6, mVertexArrayID);
		}
	}
		
	void chunkblockmodel::regChunk(chunk* Chunk)
	{
		mOurChunk = Chunk;
		mOurChunkData = Chunk->mChunkData;
	}

	void chunkblockmodel::Rebuild() //This should really be more thoroughly commented.
	{
		if(mNumFacesDrawn != 0)
		{
			for(unsigned short i = 0; i < 6; i++)
			{
				delete [] mBufferData[i];
				mBufferData[i] = 0;
				mFacesToDraw[i].clear();
				mVertexBufferID[i] = 0;
				mVertexArrayID[i] = 0;
				mTextureIDBufferID[i] = 0;
				mUVBufferID[i] = 0;
			}
			glDeleteBuffers(6, mTextureIDBufferID);
			glDeleteBuffers(6, mUVBufferID);
			glDeleteBuffers(6, mVertexBufferID);
			glDeleteVertexArrays(6, mVertexArrayID);
		}

		for(unsigned short i = 0; i < 6; i++)
		{
			mNumFacesDrawnPer[i] = 0;
		}
		mNumFacesDrawn = 0;
		for(unsigned short x = 0; x < CHUNK_SIZE; x++) //Check to see which faces of which blocks should be drawn.
		{
			for(unsigned short y = 0; y < CHUNK_SIZE; y++)
			{
				for(unsigned short z = 0; z < CHUNK_SIZE; z++)
				{
					if(mOurChunkData[((z * (CHUNK_SIZE * CHUNK_SIZE)) + (y * CHUNK_SIZE)) + x] != 0)
					{
						for(unsigned short i = 0; i < 6; i++)
						{
							mFacesToDraw[i].push_back(vector3us(x, y, z));
							mNumFacesDrawn++;
							mNumFacesDrawnPer[i]++;
						}
					}
				}
			}
		}
		if(mNumFacesDrawn != 0)
		{
			for(unsigned short SideIter = 0; SideIter < 6; SideIter++) //Build the buffer
			{
				mBufferData[SideIter] = new GLfloat[mNumFacesDrawnPer[SideIter]*FLOATS_PER_FACE];
				mTextureIDBuffer[SideIter] = new GLuint[(mNumFacesDrawnPer[SideIter] * 2 /*tris per face*/) * 3 /*verts per tri*/];
				mUVBuffer[SideIter] = new GLfloat[((mNumFacesDrawnPer[SideIter] * 2 /*tris per face*/) * 3 /*verts per tri*/) * 2 /*UV Values per vert.*/ ];
				for(unsigned short FaceIter = 0; FaceIter < mNumFacesDrawnPer[SideIter]; FaceIter++)
				{
					for(unsigned short VertIter = 0; VertIter < 6; VertIter++)
					{
						/*Input the vertex's coordinates by getting info from gSides and then adding the
						corresponding coordinate of the block (remember blocks are unit cubes).*/
						//std::cout << "SideIter:" << SideIter << std::endl;
						//std::cout << "(FaceIter*FLOATS_PER_FACE)+VertIter*3:" << SideIter << std::endl;
						//std::cout << "ResultingPosX:" << gSides[(SideIter*FLOATS_PER_FACE)+(VertIter*3)] + mFacesToDraw[SideIter].at(FaceIter).getX() << std::endl;
						//std::cout << "ResultingPosY:" << gSides[((SideIter*FLOATS_PER_FACE)+(VertIter*3))+1] + mFacesToDraw[SideIter].at(FaceIter).getY() << std::endl;
						//std::cout << "ResultingPosZ:" << gSides[((SideIter*FLOATS_PER_FACE)+(VertIter*3))+2] + mFacesToDraw[SideIter].at(FaceIter).getZ() << std::endl;
						mBufferData[SideIter][(FaceIter*FLOATS_PER_FACE)+(VertIter*3)] =
							gSides[(SideIter*FLOATS_PER_FACE)+(VertIter*3)] + mFacesToDraw[SideIter].at(FaceIter).getX();

						mBufferData[SideIter][((FaceIter*FLOATS_PER_FACE)+(VertIter*3)+1)] = 
							gSides[((SideIter*FLOATS_PER_FACE)+(VertIter*3))+1] + mFacesToDraw[SideIter].at(FaceIter).getY();

						mBufferData[SideIter][((FaceIter*FLOATS_PER_FACE)+(VertIter*3)+2)] = 
							gSides[((SideIter*FLOATS_PER_FACE)+(VertIter*3))+2] + mFacesToDraw[SideIter].at(FaceIter).getZ();

						mTextureIDBuffer[SideIter][(FaceIter * 6 /* Vertexes per face. */) + VertIter] = 1;
						
						if( (VertIter == 0) || (VertIter == 4) )
						{
							mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/] =
								1; //X Value
							mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/ + 1] =
								1; //Y Value
						}
						if( (VertIter == 5) || (VertIter == 2) )
						{
							mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/] =
								0; //X Value
							mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/ + 1] =
								0; //Y Value
						}
						if(VertIter == 1)
						{
							mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/] =
								1; //X Value
							mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/ + 1] =
								0; //Y Value
						}
						if(VertIter == 3)
						{
							mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/] =
								0; //X Value
							mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/ + 1] =
								1; //Y Value
						}
						std::cout << "SideIter:" << SideIter << std::endl;
						std::cout << "FaceIter:" << FaceIter << std::endl;
						std::cout << "VertIter:" << VertIter << std::endl;
						std::cout << "UV x:" << mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/] << std::endl;
						std::cout << "UV y:" << mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/+1] << std::endl;
					}
				}
			}
			//Give the buffer to OpenGL.
			glGenVertexArrays(6, mVertexArrayID);
			glGenBuffers(6, mVertexBufferID);
			glGenBuffers(6, mTextureIDBufferID);
			glGenBuffers(6, mUVBufferID);
			for(unsigned short i = 0; i < 6; i++)
			{
				glBindVertexArray(mVertexArrayID[i]);
				glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (mNumFacesDrawnPer[i]*FLOATS_PER_FACE), mBufferData[i], GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, mTextureIDBufferID[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * ((mNumFacesDrawnPer[i] * 2 /*tris per face*/ ) * 3 /*verts per tri*/), mTextureIDBuffer[i], GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (((mNumFacesDrawnPer[i] * 2 /*tris per face*/ ) * 3 /*verts per tri*/) * 2 /*values per vert*/), mUVBuffer[i], GL_STATIC_DRAW);
			}
		}
	}

	void chunkblockmodel::Draw()
	{
		if(mNumFacesDrawn != 0)
		{
			for(unsigned short i = 0; i < 6; i++)
			{
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID[i]);
				glVertexAttribPointer(
				   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				   3,                  // size
				   GL_FLOAT,           // type
				   GL_FALSE,           // normalized?
				   0,                  // stride
				   (void*)0            // array buffer offset
				);
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, mTextureIDBufferID[i]);
				glVertexAttribPointer(
				   1,                 
				   1,                  // size
				   GL_UNSIGNED_INT,           // type
				   GL_FALSE,           // normalized?
				   0,                  // stride
				   (void*)0            // array buffer offset
				);
				glEnableVertexAttribArray(2);
				glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID[i]);
				glVertexAttribPointer(
				   2,                  
				   2,                  // size
				   GL_FLOAT,           // type
				   GL_FALSE,           // normalized?
				   0,                  // stride
				   (void*)0            // array buffer offset
				);
				
				try
				{
					glDrawArrays(GL_TRIANGLES, 0, mNumFacesDrawnPer[i]*FLOATS_PER_FACE);
				}
				catch( ... )
				{
					std::cout << glGetError();
				}
				
				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(2);
			}
		}
	};
}