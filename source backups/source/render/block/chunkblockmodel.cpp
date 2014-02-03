#include "chunkblockmodel.h"
#include <gl/glew.h>
#include <gl/gl.h>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "..\..\system\typedefs.h"
#include "..\..\system\math\vector3r.h"
#include "blockart.h"

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
		//corresponds to Wanderlust::axis

		//POSITIVE_X:
		//-First triangle:
		POSX_POSY_NEGZ_VERT,
		POSX_POSY_POSZ_VERT,
		POSX_NEGY_POSZ_VERT,
		//-Second triangle:
		POSX_NEGY_POSZ_VERT,
		POSX_NEGY_NEGZ_VERT,
		POSX_POSY_NEGZ_VERT,

		//NEGATIVE_X
		//-First triangle:
		NEGX_POSY_POSZ_VERT,
		NEGX_POSY_NEGZ_VERT,
		NEGX_NEGY_NEGZ_VERT,
		//-Second triangle
		NEGX_NEGY_NEGZ_VERT,
		NEGX_NEGY_POSZ_VERT,
		NEGX_POSY_POSZ_VERT,

		//POSITIVE_Y
		//-First triangle:
		NEGX_POSY_NEGZ_VERT,
		NEGX_POSY_POSZ_VERT,
		POSX_POSY_POSZ_VERT,
		//-Second triangle
		POSX_POSY_POSZ_VERT,
		POSX_POSY_NEGZ_VERT,
		NEGX_POSY_NEGZ_VERT,
		
		//NEGATIVE_Y
		//-First triangle:
		POSX_NEGY_NEGZ_VERT,
		POSX_NEGY_POSZ_VERT,
		NEGX_NEGY_POSZ_VERT,
		//-Second triangle
		NEGX_NEGY_POSZ_VERT,
		NEGX_NEGY_NEGZ_VERT,
		POSX_NEGY_NEGZ_VERT,

		//POSITIVE_Z
		//-First triangle:
		POSX_POSY_POSZ_VERT,
		NEGX_POSY_POSZ_VERT,
		NEGX_NEGY_POSZ_VERT,
		//-Second triangle
		NEGX_NEGY_POSZ_VERT,
		POSX_NEGY_POSZ_VERT,
		POSX_POSY_POSZ_VERT,

		//NEGATIVE_Z
		//-First triangle:
		NEGX_POSY_NEGZ_VERT,
		POSX_POSY_NEGZ_VERT,
		POSX_NEGY_NEGZ_VERT,
		//-Second triangle
		POSX_NEGY_NEGZ_VERT,
		NEGX_NEGY_NEGZ_VERT,
		NEGX_POSY_NEGZ_VERT,
	};

#define FLOATS_PER_FACE 18
#define UV_VALUES_PER_VERT 3

	chunkblockmodel::chunkblockmodel()
	{
		for(unsigned short i = 0; i < 6; i++)
		{
			mBufferData[i] = 0;
			mVertexBufferID[i] = 0;
			mVertexArrayID[i] = 0;
			mNumFacesDrawnPer[i] = 0;
			
			mFacesToDraw[i].reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
		}
		mOurChunk = 0;
		mNumFacesDrawn = 0;
		mBlockIndex = 0;
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
	}
	void chunkblockmodel::regBlockIndex(blockindex* Index)
	{
		mBlockIndex = Index; 
	}

	bool chunkblockmodel::checkcullside(blockart* BA, axis Direction, unsigned short x, unsigned short y, unsigned short z)
	{
		if(BA != 0)
		{
			/*Get the direction opposite of the direction we're looking in, to get info about the face opposite the one 
			we're looking at.*/
			axis OppositeDirection;
			//Directions
			short CheckX = x;
			short CheckY = y;
			short CheckZ = z;
			switch (Direction)
			{
			case POSITIVE_X:
				CheckX++;
				OppositeDirection = NEGATIVE_X;
				break;
			case NEGATIVE_X:
				CheckX--;
				OppositeDirection = POSITIVE_X;
				break;
			case POSITIVE_Y:
				CheckY++;
				OppositeDirection = NEGATIVE_Y;
				break;
			case NEGATIVE_Y:
				CheckY--;
				OppositeDirection = POSITIVE_Y;
				break;
			case POSITIVE_Z:
				CheckZ++;
				OppositeDirection = NEGATIVE_Y;
				break;
			case NEGATIVE_Z:
				CheckZ--;
				OppositeDirection = POSITIVE_Y;
				break;
			}
			if((CheckX < 0) || (CheckY < 0) || (CheckZ < 0))
			{
				return false;
			}
			else if((CheckX >= CHUNK_SIZE) || (CheckY >= CHUNK_SIZE) || (CheckZ >= CHUNK_SIZE))
			{
				return false;
			}
			if(mOurChunk->getBlock(CheckX, CheckY, CheckZ) != 0)
			{
				blockart* CheckAgainst = mBlockIndex->getBlockByID(mOurChunk->getBlock(CheckX, CheckY, CheckZ))->getBlockArt();
				if(CheckAgainst != 0)
				{
					if(CheckAgainst->Visible(CheckX, CheckY, CheckZ))
					{
						if(CheckAgainst == BA)
						{
							return CheckAgainst->getCullsSelf(OppositeDirection, CheckX, CheckY, CheckZ);
						}
						else
						{
							return CheckAgainst->getCullsOthers(OppositeDirection, CheckX, CheckY, CheckZ);
						}
					}
				}
			}
			return false;
		}
		else
		{
			return true;
		}
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
			//glDeleteBuffers(6, mTextureIDBufferID);
			glDeleteBuffers(6, mUVBufferID);
			glDeleteBuffers(6, mVertexBufferID);
			glDeleteVertexArrays(6, mVertexArrayID);
		}

		for(unsigned short i = 0; i < 6; i++)
		{
			mNumFacesDrawnPer[i] = 0;
		}
		mNumFacesDrawn = 0;
		blockart* TempBlockArt = 0;
		for(unsigned short x = 0; x < CHUNK_SIZE; x++) //Check to see which faces of which blocks should be drawn.
		{
			for(unsigned short y = 0; y < CHUNK_SIZE; y++)
			{
				for(unsigned short z = 0; z < CHUNK_SIZE; z++)
				{
					if(mOurChunk->getBlock(x, y, z) != 0)
					{
						TempBlockArt = mBlockIndex->getBlockByID(mOurChunk->getBlock(x, y, z))->getBlockArt();
						if(TempBlockArt != 0)
						{
							if(TempBlockArt->Visible(x,y,z))
							{
								if(TempBlockArt->isCube(x, y, z))
								{
									for(unsigned short i = 0; i < 6; i++)
									{
										if(!checkcullside(TempBlockArt, NumToAxis[i], x, y, z))
										{
											mFacesToDraw[i].push_back(facedrawinfo(TempBlockArt, vector3us(x, y, z)));
											mNumFacesDrawn++;
											mNumFacesDrawnPer[i]++;
										}
									}
								}
								else if(TempBlockArt->isSimple()) //Do slope things. (We're not a cube but we're simple? We're a slope.)
								{
								}
								else //Do mesh things.
								{
								}

							}
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
				mUVBuffer[SideIter] = new GLfloat[((mNumFacesDrawnPer[SideIter] * 2 /*tris per face*/) * 3 /*verts per tri*/) * UV_VALUES_PER_VERT ];

				mTextureIDBuffer[SideIter] = new GLuint[(mNumFacesDrawnPer[SideIter] * 2 /*tris per face*/) * 3 /*verts per tri*/];
				
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
							gSides[(SideIter*FLOATS_PER_FACE)+(VertIter*3)] + mFacesToDraw[SideIter].at(FaceIter).Position.getX();

						mBufferData[SideIter][((FaceIter*FLOATS_PER_FACE)+(VertIter*3)+1)] = 
							gSides[((SideIter*FLOATS_PER_FACE)+(VertIter*3))+1] + mFacesToDraw[SideIter].at(FaceIter).Position.getY();

						mBufferData[SideIter][((FaceIter*FLOATS_PER_FACE)+(VertIter*3)+2)] = 
							gSides[((SideIter*FLOATS_PER_FACE)+(VertIter*3))+2] + mFacesToDraw[SideIter].at(FaceIter).Position.getZ();
						
						if((VertIter == 2) || (VertIter == 3))
						{
						mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*UV_VALUES_PER_VERT) + (VertIter*UV_VALUES_PER_VERT))] =
							0; //U Value 1
						mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*UV_VALUES_PER_VERT) + (VertIter*UV_VALUES_PER_VERT)) + 1] =
							1; //V Value 0
						}
						else if((VertIter == 0) || (VertIter == 5))
						{
						mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*UV_VALUES_PER_VERT) + (VertIter*UV_VALUES_PER_VERT))] =
							1; //U Value 0
						mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*UV_VALUES_PER_VERT) + (VertIter*UV_VALUES_PER_VERT)) + 1] =
							0; //V Value 1
						}
						else if(VertIter == 1)
						{
						mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*UV_VALUES_PER_VERT) + (VertIter*UV_VALUES_PER_VERT))] =
							0; //U Value 1
						mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*UV_VALUES_PER_VERT) + (VertIter*UV_VALUES_PER_VERT)) + 1] =
							0; //V Value 1
						}
						else if(VertIter == 4)
						{
						mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*UV_VALUES_PER_VERT) + (VertIter*UV_VALUES_PER_VERT))] =
							1; //U Value 0
						mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*UV_VALUES_PER_VERT) + (VertIter*UV_VALUES_PER_VERT)) + 1] =
							1; //V Value 0
						}
						mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*UV_VALUES_PER_VERT) + (VertIter*UV_VALUES_PER_VERT)) + 2] =
							mFacesToDraw[SideIter].at(FaceIter).Block->getTexture(SideIter, mFacesToDraw[SideIter].at(FaceIter).Position.getX(), 
							mFacesToDraw[SideIter].at(FaceIter).Position.getY(), mFacesToDraw[SideIter].at(FaceIter).Position.getZ()) - 1;

						//std::cout << "SideIter:" << SideIter << std::endl;
						//std::cout << "FaceIter:" << FaceIter << std::endl;
						//std::cout << "VertIter:" << VertIter << std::endl;
						//std::cout << "UV x:" << mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/] << std::endl;
						//std::cout << "UV y:" << mUVBuffer[SideIter][(((FaceIter * 6 /*Vertexes per face.*/)*2) /*UV values per vert*/ + (VertIter*2)) /*UV values per vert*/+1] << std::endl;
					}
				}
			}
			//Give the buffer to OpenGL.
			glGenVertexArrays(6, mVertexArrayID);
			glGenBuffers(6, mVertexBufferID);
			//glGenBuffers(6, mTextureIDBufferID);
			glGenBuffers(6, mUVBufferID);
			for(unsigned short i = 0; i < 6; i++)
			{
				glBindVertexArray(mVertexArrayID[i]);
				glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (mNumFacesDrawnPer[i]*FLOATS_PER_FACE), mBufferData[i], GL_STATIC_DRAW);
				//glBindBuffer(GL_ARRAY_BUFFER, mTextureIDBufferID[i]);
				//glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * ((mNumFacesDrawnPer[i] * 2 /*tris per face*/ ) * 3 /*verts per tri*/), mTextureIDBuffer[i], GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (((mNumFacesDrawnPer[i] * 2 /*tris per face*/ ) * 3 /*verts per tri*/) * UV_VALUES_PER_VERT), mUVBuffer[i], GL_STATIC_DRAW);
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
				);/*
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, mTextureIDBufferID[i]);
				glVertexAttribPointer(
				   1,                 
				   1,                  // size
				   GL_UNSIGNED_INT,           // type
				   GL_FALSE,           // normalized?
				   0,                  // stride
				   (void*)0            // array buffer offset
				);*/
				glEnableVertexAttribArray(2);
				glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID[i]);
				glVertexAttribPointer(
				   2,                  
				   3,                  // size
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
				//glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(2);
			}
		}
	};
}