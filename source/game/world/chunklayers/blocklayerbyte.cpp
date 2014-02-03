#include "blocklayerbyte.h"
#include <fstream>

namespace Wanderlust
{
	byteblocklayer::byteblocklayer()
	{
		for(int i = 0; i < 6; i++)
		{
			mAjacent[i] = 0;
		}
		mX = 0;
		mY = 0;
		mZ = 0;
	}
	byteblocklayer::~byteblocklayer()
	{
		for(byte i = 0; i < 6; i++) //Notify the ajacent chunks that we're going to be going away now.
		{
			if(mAjacent[i])
			{
				switch (i)
				{
				case POSITIVE_X:
					mAjacent[i]->registerAjacent(0, NEGATIVE_X);
					break;
				case NEGATIVE_X:
					mAjacent[i]->registerAjacent(0, POSITIVE_X);
					break;
				case POSITIVE_Y:
					mAjacent[i]->registerAjacent(0, NEGATIVE_Y);
					break;
				case NEGATIVE_Y:
					mAjacent[i]->registerAjacent(0, POSITIVE_Y);
					break;
				case POSITIVE_Z:
					mAjacent[i]->registerAjacent(0, NEGATIVE_Z);
					break;
				case NEGATIVE_Z:
					mAjacent[i]->registerAjacent(0, POSITIVE_Z);
					break;
				}
			}
		}
	}

	byte byteblocklayer::getBlock(unsigned short X, unsigned short Y, unsigned short Z)
	{
		if(isEmpty())
		{
			return 0;
		}
		else
		{
			if(((X < CHUNK_SIZE) && (Y < CHUNK_SIZE)) && (Z < CHUNK_SIZE))
			{
				return mBlockData[((Z*(CHUNK_SIZE * CHUNK_SIZE)) + Y*CHUNK_SIZE) + X];
			}
			else
			{
				return 0;
			}
		}
	}
	void byteblocklayer::setBlock(unsigned short X, unsigned short Y, unsigned short Z, byte NewID)
	{
		if(((X < CHUNK_SIZE) && (Y < CHUNK_SIZE)) && (Z < CHUNK_SIZE))
		{
			if(isEmpty())
			{
				if(NewID != 0)
				{
					MakeNotEmpty();
					mBlockData[((Z*(CHUNK_SIZE* CHUNK_SIZE)) + Y*CHUNK_SIZE) + X] = NewID;
				}
			}
			else
			{
				mBlockData[((Z*(CHUNK_SIZE* CHUNK_SIZE)) + Y*CHUNK_SIZE) + X] = NewID;
			}
		}
	}
	byte* byteblocklayer::getData()
	{
		return mBlockData;
	}
	size_t byteblocklayer::byteblocklayer::getDataSize()
	{
		return (CHUNK_SIZE * CHUNK_SIZE) * CHUNK_SIZE;
	}
}