#include "chunk.h"
#include <fstream>
#include "chunklayers\blocklayerbyte.h"

namespace Wanderlust
{
	typedef union _chunklayerheader
	{
		struct
		{
			unsigned short Size;
			unsigned long Revision;
		};
		char Data[sizeof(unsigned short)+sizeof(unsigned long)];
	} chunklayerheader;

	chunk::chunk()
	{
		for(int i = 0; i < 6; i++)
		{
			mAjacentChunks[i] = 0;
		}
		mX = 0;
		mY = 0;
		mZ = 0;
		mBlockLayerBase = static_cast<blocklayer*>(new byteblocklayer); //Testing purposes only.
		addLayer((chunklayer*)mBlockLayerBase);
		mBlockLayerStretch = 0;
		mDoingIO = false;
	}
	chunk::~chunk()
	{
		for(byte i = 0; i < 6; i++) //Notify the ajacent chunks that we're going to die.
		{
			if(mAjacentChunks[i])
			{
				switch (i)
				{
				case POSITIVE_X:
					mAjacentChunks[i]->registerAjacentChunk(0, NEGATIVE_X);
					break;
				case NEGATIVE_X:
					mAjacentChunks[i]->registerAjacentChunk(0, POSITIVE_X);
					break;
				case POSITIVE_Y:
					mAjacentChunks[i]->registerAjacentChunk(0, NEGATIVE_Y);
					break;
				case NEGATIVE_Y:
					mAjacentChunks[i]->registerAjacentChunk(0, POSITIVE_Y);
					break;
				case POSITIVE_Z:
					mAjacentChunks[i]->registerAjacentChunk(0, NEGATIVE_Z);
					break;
				case NEGATIVE_Z:
					mAjacentChunks[i]->registerAjacentChunk(0, POSITIVE_Z);
					break;
				}
			}
		}
		if(mLayers.size() > 0)
		{
			for(std::vector<chunklayer*>::iterator i = mLayers.begin(); i != mLayers.end(); i++) //Delete all layers.
			{
				if((*i) != 0) //But only if they exist!
				{
					delete (*i);
				}
			}
		}
	}
	
	blockID chunk::getBlock(unsigned short X, unsigned short Y, unsigned short Z)
	{
		if(mBlockLayerBase != 0)
		{
			if(mBlockLayerStretch != 0)
			{
				return mBlockLayerBase->getBlock(X, Y, Z) + (mBlockLayerStretch->getBlock(X, Y, Z)*256);
			}
			else
			{
				return mBlockLayerBase->getBlock(X, Y, Z);
			}
		}
		else
		{
			return 0;
		}
	}
	void chunk::setBlock(unsigned short X, unsigned short Y, unsigned short Z, blockID NewID)
	{
		if(NewID >= 256)
		{
			byte FirstComponent;
			FirstComponent = NewID % 256;
			if(mBlockLayerBase != 0)
			{
				mBlockLayerBase->setBlock(X, Y, Z, FirstComponent);
				if(mBlockLayerStretch != 0)
				{
					byte SecondComponent;
					SecondComponent = (NewID - FirstComponent) / 256; //I should probably use bit shifting for this but whatever, it does the same thing.
					mBlockLayerStretch->setBlock(X, Y, Z, SecondComponent);
				}
			}
		}
		else
		{
			if(mBlockLayerBase != 0)
			{
				mBlockLayerBase->setBlock(X, Y, Z, NewID);
			}
		}
	}
	void chunk::registerAjacentChunk(chunk* Neighbor, axis Direction)
	{
		mAjacentChunks[Direction] = Neighbor;
	}
	chunk* chunk::getAjacentChunk(axis Axis)
	{
		return mAjacentChunks[Axis];
	}

	void chunk::InitToZero()
	{
		for(std::vector<chunklayer*>::iterator i; i != mLayers.end(); i++)
		{
			(*i)->InitToZero();
		}
	}

	int chunk::getXPosition()
	{
		return mX;
	}
	int chunk::getYPosition()
	{
		return mY;
	}
	int chunk::getZPosition()
	{
		return mZ;
	}
	
	//ONLY the space in which this chunk exists should use these:
	void chunk::setXPosition(int x)
	{
		mX = x;
	}
	void chunk::setYPosition(int y)
	{
		mY = y;
	}
	void chunk::setZPosition(int z)
	{
		mZ = z;
	}
	unsigned short chunk::addLayer(chunklayer* Layer)
	{
		mLayers.push_back(Layer);
		mLayers.at(mLayers.size()-1)->setLayerID(mLayers.size()); //Remember: Layer ID 0 is error.
		return mLayers.size();
	}
	chunklayer* chunk::getLayer(unsigned short ID)
	{
		if(ID <= mLayers.size()) //Does this layer actually exist?
		{
			return mLayers.at(ID-1); //Remember: Layer ID 0 is error.
		}
		else
		{
			return 0;
		}
	}
	char* chunk::Save()
	{
		if(!mDoingIO)
		{
			SaveSize();
			if(mSaveSize != 0)
			{
				mDoingIO = true;
				mIOBuffer = new char[mSaveSize];
				char* ByteIter = mIOBuffer;
				char* End = mIOBuffer + mSaveSize;
				chunklayerheader Header;
				for(std::vector<chunklayer*>::iterator i = mLayers.begin(); i != mLayers.end(); i++)
				{
					if(ByteIter >= End)
					{
						break;
					}
					if((*i)->isEmpty())
					{
						Header.Size = 0;
					}
					else
					{
						Header.Size = (*i)->SaveSize();
					}
					memcpy((void*)ByteIter,(void*)&Header, sizeof(chunklayerheader));
					ByteIter += sizeof(chunklayerheader);
					if(ByteIter >= End)
					{
						break;
					}
					if(Header.Size != 0)
					{
						memcpy((void*)ByteIter, (void*)(*i)->Save(), Header.Size);
						ByteIter += Header.Size;
						(*i)->DoneSaving();
					}
					if(ByteIter >= End)
					{
						break;
					}
				}
				return mIOBuffer;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	unsigned int chunk::SaveSize()
	{
		mSaveSize = 0;
		for(std::vector<chunklayer*>::iterator i = mLayers.begin(); i != mLayers.end(); i++)
		{
			mSaveSize += sizeof(chunklayerheader);
			mSaveSize += (*i)->SaveSize();
		}
		return mSaveSize;
	}
	void chunk::DoneSaving()
	{
		if(mDoingIO)
		{
			delete [] mIOBuffer;
			mDoingIO = false;
		}
	}

	unsigned int chunk::Load(char* Buffer, unsigned int Size)
	{
		if(!mDoingIO)
		{
			//mIOBuffer = new char[Size];
			if(Size > sizeof(chunklayerheader))
			{
				mDoingIO = true;
				chunklayerheader Header;
				char* ByteIter = Buffer;
				char* End = Buffer + Size;
				for(std::vector<chunklayer*>::iterator i = mLayers.begin(); i != mLayers.end(); i++)
				{
					memcpy((void*)&Header, (void*)ByteIter, sizeof(chunklayerheader));
					ByteIter += sizeof(chunklayerheader);
					if(ByteIter >= End)
					{
						break;
					}
					if(Header.Size == 0)
					{
						(*i)->setEmpty(true);
					}
					else
					{
						char* LayerBuffer = (*i)->Load(Header.Size);
						if(LayerBuffer != 0)
						{
							memcpy((void*)LayerBuffer, (void*)ByteIter, Header.Size);
							(*i)->DoneLoading();
						}
					}
					ByteIter += Header.Size;
					if(ByteIter >= End)
					{
						break;
					}
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	void chunk::DoneLoading()
	{
		if(mDoingIO)
		{
			//delete [] mIOBuffer;
			mDoingIO = false;
		}
	}
}