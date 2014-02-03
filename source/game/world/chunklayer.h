#ifndef CHUNK_LAYER_H
#define CHUNK_LAYER_H

#include "constants.h"
#include "..\..\system\typedefs.h"
#include "..\..\system\misc\datachain.h"

namespace Wanderlust
{
	class chunk;
	class chunklayer
	{
	public:
		chunklayer();
		~chunklayer();

		virtual void InitToZero(){};
		
		virtual void registerAjacent(chunklayer*, axis);
		virtual chunklayer* getAjacent(axis);

		virtual int getXPosition();
		virtual int getYPosition();
		virtual int getZPosition();
		
		virtual bool isEmpty(); /*A layer is empty if there is no entry for it in the file for the chunk.
								Generally that means there is no element for this layer in the chunk, and
								generally that means returning 0 for every get<Element>() function.*/
		
		//ONLY the space in which this chunk layer exists should use these:
		virtual void setXPosition(int);
		virtual void setYPosition(int);
		virtual void setZPosition(int);
		virtual void setEmpty(bool);
		
		void setLayerID(unsigned short);
		unsigned short getLayerID();

		virtual void NotifyChange(short, short, short){};
		//-----------------------------------------------------------------

		virtual void regChunk(chunk*);
		virtual chunk* getChunk();
		
		virtual byte* getData(){};
		virtual size_t getDataSize(){};

	protected:
		virtual void MakeNotEmpty();
		bool mEmpty;

		chunk* mOurChunk;

		chunklayer* mAjacent[6];

		bool mStatic; //Is this terrain, and not part of a vehicle / elevator / etc..?
		int mX, mY, mZ;

		bool mDoingIO; //Are we in the process of loading or saving? If so, don't start doing either.
	private:
		unsigned short mLayerID;
	};
}

#endif