#ifndef SPACE_H
#define SPACE_H

#include "..\..\system\typedefs.h"
#include "constants.h"

//#include "chunkoverlay.h"
#include "chunk.h"
#include "worldgen\chunkgenerator.h"
#include "loadersphere.h"
#include "chunkcallback.h"

#include <map>
#include <tuple>
#include <list>
using namespace std;



namespace Wanderlust
{
	/*
	A space divided into chunks. Handles all terrain for a world. Inherited to create vehicle.
	*/
	/*
	Eventually all saving and loading will be done through a separate thread. Chunks to be unloaded
	and saved will be passed to a separate chunk i/o class, and then removed from mTerrain.
	Chunks in the process of being loaded will be initialized to 0. The same will be true of chunks  
	to be generated, with the generator also being run in a separate thread.

	When such an operation is finished, the chunk i/o class takes the pointer it has to the space
	that spawned it, says "your order, sir," and then gets destroyed by the space.
	*/
	class space
	{
	public:
		space();
		~space();
		//virtual chunk* getChunk(real, real, real); //Takes entity position as argument. Returns null if the chunk does not exist.
		virtual chunk* getChunk(int, int, int); //Chunk position as argument. Returns void if the chunk does not exist.
		virtual blockID getBlock(long int, long int, long int); /*X, Y, Z. Returns 0 if the chunk isn't loaded, and you
																			can't tell the difference between that and air, so watch out.*/		
		virtual void setBlock(long int, long int, long int, blockID); /*X, Y, Z, ID.*/
		virtual bool getBlockExists(long int, long int, long int); /*X, Y, Z. Is the chunk this block should be in loaded?*/
		//virtual blockID* getBlockPtr(long int, long int, long int);
		//virtual blockID* NotifyBlockChange(long int, int, long int); /*X, Y, Z. Notifies the chunk holding the block with the coordinates
		//															 given about the fact that the block has been changed.*/
		virtual bool GenChunk(int, int, int); //Generates chunks at a certain position. If there is no generator, return false.
		virtual void RegChunkGen(chunkgenerator*);

		virtual bool LoadChunk(int, int, int); //Loads a chunk at a certain position. If it doesn't exist, return null.
		virtual bool ForceChunk(int, int, int); /*Get a chunk if it's loaded, load it if it isn't, generate it if it doesn't exist.
												  Will only return null if there is no generator registered.*/
		virtual bool UnloadChunk(int, int, int); //Unloads a chunk from memory at the given position. Returns true if save successful.
		virtual bool SaveChunk(int, int, int); //Saves a chunk at the given position. Returns true if save successful.

		virtual void setDirectory(std::string);

		virtual void SaveAll(); //Saves all currently loaded chunks.
		virtual void UnloadAll(); //Saves and then erases all currently loaded chunks.
		virtual void Clear(); //Erases all currently loaded chunks.

		virtual void UpdateLoaded(); //Ensures that all chunks within loader spheres are loaded, and all chunks outside, unloaded.
		virtual void AddLoaderSphere(loadersphere*);
		virtual void RemoveLoaderSphere(loadersphere*);
		
		virtual void AddChunkLoadCallback(chunkcallback*);
		virtual void AddChunkUnloadCallback(chunkcallback*);
		virtual void AddChunkGenCallback(chunkcallback*);
		virtual void RemoveChunkLoadCallback(chunkcallback*);
		virtual void RemoveChunkUnloadCallback(chunkcallback*);
		virtual void RemoveChunkGenCallback(chunkcallback*);
	protected:
		class chunkposition //Only used by space as the key for its map of chunks.
		{
		public:
			chunkposition()
			{
				X = 0;
				Y = 0;
				Z = 0;
			};

			chunkposition(int x, int y, int z)
			{
				X = x;
				Y = y;
				Z = z;
			};

			int X, Y, Z;
	
			bool operator<( const chunkposition& CompareTo) const
			{
				if(Y != CompareTo.Y)
				{
					return Y < CompareTo.Y;
				}
				if(X != CompareTo.X)
				{
					return X < CompareTo.X;
				}
				return Z < CompareTo.Z;
			};
			bool operator==( const chunkposition& CompareTo) const
			{
				return ((X == CompareTo.X) && Y == (CompareTo.Y)) && Z == CompareTo.Z; 
			};
		};
		virtual void CalcChunkAjacencies(chunkposition);
		std::map<chunkposition, chunk*> mTerrain;
		std::list<loadersphere*> mKeepLoaded;

		chunkgenerator* mChunkGenPtr;

		std::string mDirectory;

		std::list<chunkcallback*> mChunkLoadCallbacks;
		std::list<chunkcallback*> mChunkUnloadCallbacks;
		std::list<chunkcallback*> mChunkGenCallbacks;
	};
}

#endif