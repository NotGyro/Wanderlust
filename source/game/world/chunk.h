#ifndef CHUNK_H
#define CHUNK_H

#include "constants.h"
//#include "chunkoverlay.h"
#include "..\..\system\typedefs.h"

#include "chunklayer.h"
#include "chunklayers\blocklayerabstract.h"
#include "..\..\system\misc\datachain.h"

#include <vector>

namespace Wanderlust
{
	class chunk
	{
	public:
		chunk();
		~chunk();

		blockID getBlock(unsigned short, unsigned short, unsigned short); //X, Y, Z
		void setBlock(unsigned short, unsigned short, unsigned short, blockID); //X, Y, Z, data.

		virtual void InitToZero();
		
		virtual void registerAjacentChunk(chunk*, axis);/*Only use this with the direction enum. Start using ints
															directly and it will be a trainwreck.*/
		virtual chunk* getAjacentChunk(axis);/*Only use this with the direction enum. Start using ints
													directly and it will be a trainwreck.*/
		virtual int getXPosition();
		virtual int getYPosition();
		virtual int getZPosition();
		//virtual bool isEmpty();
		
		//ONLY the space in which this chunk exists should use these:
		virtual void setXPosition(int);
		virtual void setYPosition(int);
		virtual void setZPosition(int);

		virtual unsigned short addLayer(chunklayer*); /*Don't be decieved by the fact that chunk layer data comes in from
											outside of the chunk class - the chunk class absolutely handles
											deleting its layers. Returns layer ID.*/
		virtual chunklayer* getLayer(unsigned short); //LayerID

		virtual size_t LayerCount();
	protected:
		std::vector<chunklayer*> mLayers; /*A layer ID, as passed around, is equal to the index of the layer in
										  mLayers + 1. A layer ID of 0 implies an error.*/
		blocklayer* mBlockLayerBase;
		blocklayer* mBlockLayerStretch;

		chunk* mAjacentChunks[6];
		bool mStatic; //Is this terrain, and not part of a vehicle / elevator / etc..?
		int mX, mY, mZ;
		
		bool mDoingIO; //Are we in the process of loading or saving? If so, don't start doing either.

		unsigned int mSaveSize;
		char* mIOBuffer;
	};
}

#endif