#ifndef CHUNK_OVERLAY_H
#define CHUNK_OVERLAY_H

#include "..\..\system\typedefs.h"

namespace Wanderlust
{
	class chunk; //For registration purposes.
	class chunkoverlaymanager; //And the same.

	/*These should be implemented such that doing something to an overlay in a thread can happen
	while something else is done to another overlay while something is done to the chunk.*/
	class chunkoverlay //Should be inherited, never used directly.
	{
	public:
		chunkoverlay();
		~chunkoverlay();

		virtual bool InitLoading(unsigned int BuffSize){return false;}; /*Returns whether or not this is allowed. The argument is the size of the buffer for 
										  pulling data from the chunk.*/
		virtual void Load(byte* Buffer){}; //Load from buffer.
		virtual void DoneLoading(){};

		virtual bool InitSaving(unsigned int BuffSize){return false;}; /*Returns whether or not this is allowed. The argument is the size of the buffer for
										 feeding data to the chunk.*/
		virtual void Save(byte* Buffer){}; //Save to buffer.
		virtual unsigned long DoneSaving(){}; //Return value is the total size in bytes of saved data.

		virtual bool isEmpty(){return true;};
		
		virtual void RegisterOurChunk(chunk*);
		virtual void RegisterManager(chunkoverlaymanager*); /*Only call this AFTER you've given the chunk overlay a position.*/
		virtual chunkoverlaymanager* getManager();
		virtual chunk* getOurChunk();
		virtual void NotifyChange(unsigned short, unsigned short, unsigned short){};
	protected:
		int mX, mY, mZ;
		bool mEmptyOverlay;
		chunk* mOurChunk;
		chunkoverlaymanager* mManager;
	};
}

#endif