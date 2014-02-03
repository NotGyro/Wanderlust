#include "chunklayer.h"
#include <fstream>

namespace Wanderlust
{
	chunklayer::chunklayer()
	{
		for(int i = 0; i < 6; i++)
		{
			mAjacent[i] = 0;
		}
		mX = 0;
		mY = 0;
		mZ = 0;
	}
	chunklayer::~chunklayer()
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

	void chunklayer::registerAjacent(chunklayer* Neighbor, axis Direction)
	{
		mAjacent[Direction] = Neighbor;
	}
	chunklayer* chunklayer::getAjacent(axis Axis)
	{
		return mAjacent[Axis];
	}

	int chunklayer::getXPosition()
	{
		return mX;
	}
	int chunklayer::getYPosition()
	{
		return mY;
	}
	int chunklayer::getZPosition()
	{
		return mZ;
	}
	
	//ONLY the space in which this chunk exists should use these:
	void chunklayer::setXPosition(int x)
	{
		mX = x;
	}
	void chunklayer::setYPosition(int y)
	{
		mY = y;
	}
	void chunklayer::setZPosition(int z)
	{
		mZ = z;
	}
	bool chunklayer::isEmpty()
	{
		return mEmpty;
	}
	void chunklayer::setEmpty(bool B)
	{
		mEmpty = B;
	}
	void chunklayer::regChunk(chunk* C)
	{
		mOurChunk = C;
	}
	chunk* chunklayer::getChunk()
	{
		return mOurChunk;
	}

	void chunklayer::MakeNotEmpty()
	{
		mEmpty = false;
		InitToZero();
	}

	void chunklayer::setLayerID(unsigned short ID)
	{
		mLayerID = ID;
	}
	unsigned short chunklayer::getLayerID()
	{
		return mLayerID;
	}
}