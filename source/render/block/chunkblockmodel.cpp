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

#define FLOATS_PER_VERTEX 3
#define FLOATS_PER_VERTEX_UVL 3
#define VERTEXES_PER_FACE 6
#define FLOATS_PER_FACE VERTEXES_PER_FACE * FLOATS_PER_VERTEX
#define FLOATS_PER_FACE_UVL VERTEXES_PER_FACE * FLOATS_PER_VERTEX_UVL

	chunkblockmodel::chunkblockmodel()
	{
		mOurChunk = 0;
		mNumFacesDrawn = 0;
		mBlockIndex = 0;
	}
	chunkblockmodel::~chunkblockmodel()
	{
		if(mNumFacesDrawn != 0)
		{
			glDeleteVertexArrays(6, mSideVertexArrayID);
			glDeleteBuffers(6, mSideVertexBufferID);
			glDeleteBuffers(6, mSideUVLBufferID);

			glDeleteVertexArrays(1, &mCoreVertexArrayID);
			glDeleteBuffers(1, &mCoreVertexBufferID);
			glDeleteBuffers(1, &mCoreUVLBufferID);
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
	
	bool chunkblockmodel::checkcullface(blockart* BA, axis Direction, unsigned short x, unsigned short y, unsigned short z)
	{
		if(BA != 0)
		{
			/*Get the direction opposite of the direction we're looking in, to get info about the face opposite the one 
			we're looking at.*/
			chunk* CheckFrom = mOurChunk;
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
				OppositeDirection = NEGATIVE_Z;
				break;
			case NEGATIVE_Z:
				CheckZ--;
				OppositeDirection = POSITIVE_Z;
			}
			if((CheckX < 0) || (CheckY < 0) || (CheckZ < 0))
			{
				CheckFrom = mOurChunk->getAjacentChunk(Direction);
				if(CheckFrom)
				{
					switch (Direction)
					{
					case NEGATIVE_X:
						CheckX = CHUNK_SIZE + CheckX;
						break;
					case NEGATIVE_Y:
						CheckY = CHUNK_SIZE + CheckY;
						break;
					case NEGATIVE_Z:
						CheckZ = CHUNK_SIZE + CheckZ;
					}
				}
				else
				{
					return false;
				}
			}
			else if((CheckX >= CHUNK_SIZE) || (CheckY >= CHUNK_SIZE) || (CheckZ >= CHUNK_SIZE))
			{
				CheckFrom = mOurChunk->getAjacentChunk(Direction);
				if(CheckFrom)
				{
					switch (Direction)
					{
					case POSITIVE_X:
						CheckX = CheckX - CHUNK_SIZE;
						break;
					case POSITIVE_Y:
						CheckY = CheckY - CHUNK_SIZE;
						break;
					case POSITIVE_Z:
						CheckZ = CheckZ - CHUNK_SIZE;
					}
				}
				else
				{
					return false;
				}
			}
			if(CheckFrom->getBlock(CheckX, CheckY, CheckZ) != 0)
			{
				blockart* CheckAgainst = mBlockIndex->getBlockByID(CheckFrom->getBlock(CheckX, CheckY, CheckZ))->getBlockArt();
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
	bool chunkblockmodel::isOnSide(axis Direction, unsigned short x, unsigned short y, unsigned short z)
	{
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
			OppositeDirection = NEGATIVE_Z;
			break;
		case NEGATIVE_Z:
			CheckZ--;
			OppositeDirection = POSITIVE_Z;
		}
		if(((CheckX < 0) || (CheckY < 0) || (CheckZ < 0))  ||  ((CheckX >= CHUNK_SIZE) || (CheckY >= CHUNK_SIZE) || (CheckZ >= CHUNK_SIZE)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void chunkblockmodel::Rebuild() //This should really be more thoroughly commented.
	{
		if(mNumFacesDrawn != 0)
		{
		}
		
		GLfloat* CoreVertex = mCoreBufferData;
		GLfloat* CoreUVL = mCoreUVLBuffer;
		GLfloat* EdgeVertex[6];
		GLfloat* EdgeUVL[6];

		for(unsigned short i = 0; i < 6; i++)
		{
			EdgeVertex[i] = mSideBufferData[i];
			EdgeUVL[i] = mSideUVLBuffer[i];

			mSideNumFacesDrawn[i] = 0;
		}
		mCoreNumFacesDrawn = 0;
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
									for(unsigned short Direction = 0; Direction < 6; Direction++)
									{
										if(!checkcullface(TempBlockArt, NumToAxis[Direction], x, y, z))
										{
											++mNumFacesDrawn;
											if(isOnSide(NumToAxis[Direction], x, y, z))
											{
												++mSideNumFacesDrawn[Direction];
												for(unsigned short VertIter = 0; VertIter < VERTEXES_PER_FACE; VertIter++)
												{
													/*Input the vertex's coordinates by getting info from gSides and then adding the
													corresponding coordinate of the block (remember blocks are unit cubes).*/
													EdgeVertex[Direction][0] = gSides[(Direction*FLOATS_PER_FACE)+(VertIter*3)] + x;
												
													EdgeVertex[Direction][1] = gSides[(Direction*FLOATS_PER_FACE)+(VertIter*3)+1] + y;
													EdgeVertex[Direction][2] = gSides[(Direction*FLOATS_PER_FACE)+(VertIter*3)+2] + z;
													EdgeVertex[Direction] += FLOATS_PER_VERTEX;
						
													if((VertIter == 2) || (VertIter == 3))
													{
														EdgeUVL[Direction][0] = 0;
														EdgeUVL[Direction][1] = 1;
													}
													else if((VertIter == 0) || (VertIter == 5))
													{
														EdgeUVL[Direction][0] = 1;
														EdgeUVL[Direction][1] = 0;
													}
													else if(VertIter == 1)
													{
														EdgeUVL[Direction][0] = 0;
														EdgeUVL[Direction][1] = 0;
													}
													else if(VertIter == 4)
													{
														EdgeUVL[Direction][0] = 1;
														EdgeUVL[Direction][1] = 1;
													}
													EdgeUVL[Direction][2] = TempBlockArt->getTexture(Direction, x, y, z) - 1;
													EdgeUVL[Direction] += FLOATS_PER_VERTEX_UVL;
												}
											}
											else
											{
												++mCoreNumFacesDrawn;
												for(unsigned short VertIter = 0; VertIter < VERTEXES_PER_FACE; VertIter++)
												{
													/*Input the vertex's coordinates by getting info from gSides and then adding the
													corresponding coordinate of the block (remember blocks are unit cubes).*/
													CoreVertex[0] = gSides[(Direction*FLOATS_PER_FACE)+(VertIter*3)] + x;
												
													CoreVertex[1] = gSides[(Direction*FLOATS_PER_FACE)+(VertIter*3)+1] + y;
													CoreVertex[2] = gSides[(Direction*FLOATS_PER_FACE)+(VertIter*3)+2] + z;
													CoreVertex += FLOATS_PER_VERTEX;
						
													if((VertIter == 2) || (VertIter == 3))
													{
														CoreUVL[0] = 0;
														CoreUVL[1] = 1;
													}
													else if((VertIter == 0) || (VertIter == 5))
													{
														CoreUVL[0] = 1;
														CoreUVL[1] = 0;
													}
													else if(VertIter == 1)
													{
														CoreUVL[0] = 0;
														CoreUVL[1] = 0;
													}
													else if(VertIter == 4)
													{
														CoreUVL[0] = 1;
														CoreUVL[1] = 1;
													}
													CoreUVL[2] = TempBlockArt->getTexture(Direction, x, y, z) - 1;
													CoreUVL += FLOATS_PER_VERTEX_UVL;
												}
											}
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
			//Give the buffer to OpenGL.
			glDeleteVertexArrays(6, mSideVertexArrayID);
			glDeleteBuffers(6, mSideVertexBufferID);
			glDeleteBuffers(6, mSideUVLBufferID);

			glDeleteVertexArrays(1, &mCoreVertexArrayID);
			glDeleteBuffers(1, &mCoreVertexBufferID);
			glDeleteBuffers(1, &mCoreUVLBufferID);

			glGenVertexArrays(6, mSideVertexArrayID);
			glGenBuffers(6, mSideVertexBufferID);
			glGenBuffers(6, mSideUVLBufferID);

			glGenVertexArrays(1, &mCoreVertexArrayID);
			glGenBuffers(1, &mCoreVertexBufferID);
			glGenBuffers(1, &mCoreUVLBufferID);
			if(mCoreNumFacesDrawn != 0)
			{
				glBindVertexArray(mCoreVertexArrayID);
				glBindBuffer(GL_ARRAY_BUFFER, mCoreVertexBufferID);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (mCoreNumFacesDrawn*FLOATS_PER_FACE), mCoreBufferData, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, mCoreUVLBufferID);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (mCoreNumFacesDrawn * FLOATS_PER_FACE_UVL), mCoreUVLBuffer, GL_STATIC_DRAW);
			}
			for(unsigned short i = 0; i < 6; i++)
			{
				if(mSideNumFacesDrawn[i] != 0)
				{
					glBindVertexArray(mSideVertexArrayID[i]);
					glBindBuffer(GL_ARRAY_BUFFER, mSideVertexBufferID[i]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (mSideNumFacesDrawn[i]*FLOATS_PER_FACE), mSideBufferData[i], GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, mSideUVLBufferID[i]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (mSideNumFacesDrawn[i] * FLOATS_PER_FACE_UVL), mSideUVLBuffer[i], GL_STATIC_DRAW);
				}
			}
		}
	}
	void chunkblockmodel::Rebuild(axis Direction) //This should really be more thoroughly commented.
	{
		GLfloat* EdgeVertex = mSideBufferData[Direction];
		GLfloat* EdgeUVL = mSideUVLBuffer[Direction];

		mNumFacesDrawn -= mSideNumFacesDrawn[Direction];

		mSideNumFacesDrawn[Direction] = 0;
		
		blockart* TempBlockArt = 0;
		
		unsigned short StartX = 0;
		unsigned short StartY = 0;
		unsigned short StartZ = 0;
		unsigned short EndX = CHUNK_SIZE;
		unsigned short EndY = CHUNK_SIZE;
		unsigned short EndZ = CHUNK_SIZE;
		for(unsigned short x = StartX; x < EndX; x++)
		{
			for(unsigned short y = StartY; y < EndY; y++)
			{
				for(unsigned short z = StartZ; z < EndZ; z++)
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
									if(!checkcullface(TempBlockArt, NumToAxis[Direction], x, y, z))
									{
										if(isOnSide(NumToAxis[Direction], x, y, z))
										{
											++mSideNumFacesDrawn[Direction];
											for(unsigned short VertIter = 0; VertIter < VERTEXES_PER_FACE; VertIter++)
											{
												/*Input the vertex's coordinates by getting info from gSides and then adding the
												corresponding coordinate of the block (remember blocks are unit cubes).*/
												EdgeVertex[0] = gSides[(Direction*FLOATS_PER_FACE)+(VertIter*3)] + x;
											
												EdgeVertex[1] = gSides[(Direction*FLOATS_PER_FACE)+(VertIter*3)+1] + y;
												EdgeVertex[2] = gSides[(Direction*FLOATS_PER_FACE)+(VertIter*3)+2] + z;
												EdgeVertex += FLOATS_PER_VERTEX;
						
												if((VertIter == 2) || (VertIter == 3))
												{
													EdgeUVL[0] = 0;
													EdgeUVL[1] = 1;
												}
												else if((VertIter == 0) || (VertIter == 5))
												{
													EdgeUVL[0] = 1;
													EdgeUVL[1] = 0;
												}
												else if(VertIter == 1)
												{
													EdgeUVL[0] = 0;
													EdgeUVL[1] = 0;
												}
												else if(VertIter == 4)
												{
													EdgeUVL[0] = 1;
													EdgeUVL[1] = 1;
												}
												EdgeUVL[2] = TempBlockArt->getTexture(Direction, x, y, z) - 1;
												EdgeUVL += FLOATS_PER_VERTEX_UVL;
											}
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
		if(mSideNumFacesDrawn[Direction] != 0)
		{
			//Give the buffer to OpenGL.
			glDeleteVertexArrays(1, &mSideVertexArrayID[Direction]);
			glDeleteBuffers(1, &mSideVertexBufferID[Direction]);
			glDeleteBuffers(1, &mSideUVLBufferID[Direction]);

			glGenVertexArrays(1, &mSideVertexArrayID[Direction]);
			glGenBuffers(1, &mSideVertexBufferID[Direction]);
			glGenBuffers(1, &mSideUVLBufferID[Direction]);

			glBindVertexArray(mSideVertexArrayID[Direction]);
			glBindBuffer(GL_ARRAY_BUFFER, mSideVertexBufferID[Direction]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (mSideNumFacesDrawn[Direction]*FLOATS_PER_FACE), mSideBufferData[Direction], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, mSideUVLBufferID[Direction]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (mSideNumFacesDrawn[Direction] * FLOATS_PER_FACE_UVL), mSideUVLBuffer[Direction], GL_STATIC_DRAW);
			mNumFacesDrawn += mSideNumFacesDrawn[Direction];
		}
	}

	void chunkblockmodel::Draw()
	{
		if(mNumFacesDrawn != 0)
		{
			if(mCoreNumFacesDrawn != 0)
			{
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, mCoreVertexBufferID);
				glVertexAttribPointer(
				   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				   3,                  // size
				   GL_FLOAT,           // type
				   GL_FALSE,           // normalized?
				   0,                  // stride
				   (void*)0            // array buffer offset
				);
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, mCoreUVLBufferID);
				glVertexAttribPointer(
				   1,                  
				   3,                  // size
				   GL_FLOAT,           // type
				   GL_FALSE,           // normalized?
				   0,                  // stride
				   (void*)0            // array buffer offset
				);
				
				glDrawArrays(GL_TRIANGLES, 0, mCoreNumFacesDrawn*FLOATS_PER_FACE);
				
				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
			}
			for(unsigned short i = 0; i < 6; i++)
			{
				if(mSideNumFacesDrawn[i] != 0)
				{
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, mSideVertexBufferID[i]);
					glVertexAttribPointer(
					   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
					   3,                  // size
					   GL_FLOAT,           // type
					   GL_FALSE,           // normalized?
					   0,                  // stride
					   (void*)0            // array buffer offset
					);
					glEnableVertexAttribArray(1);
					glBindBuffer(GL_ARRAY_BUFFER, mSideUVLBufferID[i]);
					glVertexAttribPointer(
					   1,                  
					   3,                  // size
					   GL_FLOAT,           // type
					   GL_FALSE,           // normalized?
					   0,                  // stride
					   (void*)0            // array buffer offset
					);
				
					glDrawArrays(GL_TRIANGLES, 0, mSideNumFacesDrawn[i]*FLOATS_PER_FACE);
				
					glDisableVertexAttribArray(0);
					//glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(1);
				}
			}
		}
	};
}