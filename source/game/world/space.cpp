#include "space.h"
#include <math.h>
#include <iostream>
#include "..\..\system\misc\intstrbase64.h"
#include "..\..\system\math\distance3d.h"
#include "..\..\system\typedefs.h"

namespace Wanderlust
{
	space::space()
	{
		mChunkGenPtr = 0;
		mTerrain.clear();
	}
	space::~space()
	{
		UnloadAll();
	}/*
	chunk* space::getChunk(real X, real Y, real Z)
	{
		if(mTerrain.find(chunkposition(X, Y, Z)) != mTerrain.end())
		{
			return mTerrain.find(chunkposition(X, Y, Z))->second;
		}
		else
		{
			return 0;
		}
	}*/
	chunk* space::getChunk(int X, int Y, int Z)
	{
		if(mTerrain.find(chunkposition(X, Y, Z)) != mTerrain.end())
		{
			return mTerrain.find(chunkposition(X, Y, Z))->second;
		}
		else
		{
			return 0;
		}
	}
	blockID space::getBlock(long int X, long int Y, long int Z)
	{
		real ChunkX, ChunkY, ChunkZ;
		ChunkX = (real)X/CHUNK_SIZE;
		ChunkY = (real)Y/CHUNK_SIZE;
		ChunkZ = (real)Z/CHUNK_SIZE;
		
		ChunkX = floor(ChunkX);
		ChunkY = floor(ChunkY);
		ChunkZ = floor(ChunkZ);
		chunk* FromWhich = getChunk(ChunkX, ChunkY, ChunkZ);
		if(FromWhich)
		{
			X %= CHUNK_SIZE; 
			Y %= CHUNK_SIZE; 
			Z %= CHUNK_SIZE;

			if(X < 0)
			{
				X = CHUNK_SIZE + X;
			}
			if(Y < 0)
			{
				Y = CHUNK_SIZE + Y;
			}
			if(Z < 0)
			{
				Z = CHUNK_SIZE + Z;
			}
			return FromWhich->getBlock(X, Y, Z);
		}
		else
		{
			return 0;
		}
	}
	bool space::getBlockExists(long int X, long int Y, long int Z)
	{
		real ChunkX, ChunkY, ChunkZ;
		ChunkX = (real)X/CHUNK_SIZE;
		ChunkY = (real)Y/CHUNK_SIZE;
		ChunkZ = (real)Z/CHUNK_SIZE;
		
		ChunkX = floor(ChunkX);
		ChunkY = floor(ChunkY);
		ChunkZ = floor(ChunkZ);
		chunk* ToWhich = getChunk(ChunkX, ChunkY, ChunkZ);
		if(ToWhich)
		{
			return true;
		}
		return false;
	}
	void space::setBlock(long int X, long int Y, long int Z, blockID SetTo)
	{
		real ChunkX, ChunkY, ChunkZ;
		ChunkX = (real)X/CHUNK_SIZE;
		ChunkY = (real)Y/CHUNK_SIZE;
		ChunkZ = (real)Z/CHUNK_SIZE;
		
		ChunkX = floor(ChunkX);
		ChunkY = floor(ChunkY);
		ChunkZ = floor(ChunkZ);
		chunk* ToWhich = getChunk(ChunkX, ChunkY, ChunkZ);
		if(ToWhich)
		{
			X %= CHUNK_SIZE; 
			Y %= CHUNK_SIZE; 
			Z %= CHUNK_SIZE;

			if(X < 0)
			{
				X = CHUNK_SIZE + X;
			}
			if(Y < 0)
			{
				Y = CHUNK_SIZE + Y;
			}
			if(Z < 0)
			{
				Z = CHUNK_SIZE + Z;
			}
			ToWhich->setBlock(X, Y, Z, SetTo);
		}
	}
	/*blockID* space::getBlockPtr(long int X, long int Y, long int Z)
	{
		real ChunkX, ChunkY, ChunkZ;
		ChunkX = (real)X/CHUNK_SIZE;
		ChunkY = (real)Y/CHUNK_SIZE;
		ChunkZ = (real)Z/CHUNK_SIZE;
		
		ChunkX = floor(ChunkX);
		ChunkY = floor(ChunkY);
		ChunkZ = floor(ChunkZ);
		chunk* FromWhich = getChunk(ChunkX, ChunkY, ChunkZ);
		if(FromWhich)
		{
			X %= CHUNK_SIZE; 
			Y %= CHUNK_SIZE; 
			Z %= CHUNK_SIZE;

			if(X < 0)
			{
				X = CHUNK_SIZE + X;
			}
			if(Y < 0)
			{
				Y = CHUNK_SIZE + Y;
			}
			if(Z < 0)
			{
				Z = CHUNK_SIZE + Z;
			}
			return FromWhich->getBlockPtr(X, Y, Z);
		}
		else
		{
			return 0;
		}
	}*/

	bool space::GenChunk(int X, int Y, int Z)
	{
		if(mTerrain.find(chunkposition(X, Y, Z)) == mTerrain.end())
		{
			if(mChunkGenPtr)
			{
				mTerrain[chunkposition(X, Y, Z)] = new chunk;
				mTerrain.find(chunkposition(X, Y, Z))->second->setXPosition(X);
				mTerrain.find(chunkposition(X, Y, Z))->second->setYPosition(Y);
				mTerrain.find(chunkposition(X, Y, Z))->second->setZPosition(Z);
				mChunkGenPtr->GenerateChunk(mTerrain.find(chunkposition(X, Y, Z))->second);
				CalcChunkAjacencies(chunkposition(X, Y, Z));
				for(std::list<chunkcallback*>::iterator i = mChunkGenCallbacks.begin(); i != mChunkGenCallbacks.end(); i++)
				{
					(*i)->Callback(getChunk(X, Y, Z));
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		return false;
	}
	
	void space::RegChunkGen(chunkgenerator* Generator)
	{
		mChunkGenPtr = Generator;
	}
	

	void space::setDirectory(std::string SetTo)
	{
		mDirectory = SetTo;
	}
	bool space::LoadChunk(int X, int Y, int Z)
	{
		bool Success = false;/*
		std::string Filename = mDirectory;
		
		Filename.append(IntToBase64(X));
		Filename.append(" ");
		Filename.append(IntToBase64(Y));
		Filename.append(" ");
		Filename.append(IntToBase64(Z));
		Filename.append(".wlc");

		datachainnode* LoaderStructure = LoadDataChainStructureFromFile(Filename);
		if(LoaderStructure != 0)
		{
			datachainnode* Loader = getChunk(X, Y, Z)->getLoader(LoaderStructure);
			if(Loader != 0)
			{
				Success = LoadDataChainDataFromFile(Filename, Loader);
			}
			getChunk(X, Y, Z)->DoneLoading();
			DeleteDataChainStructure(Loader);
			DeleteDataChainStructure(LoaderStructure);
			
			getChunk(X, Y, Z)->setXPosition(X);
			getChunk(X, Y, Z)->setYPosition(Y);
			getChunk(X, Y, Z)->setZPosition(Z);
			CalcChunkAjacencies(chunkposition(X, Y, Z));
			for(std::list<chunkcallback*>::iterator i = mChunkLoadCallbacks.begin(); i != mChunkLoadCallbacks.end(); i++)
			{
				(*i)->Callback(getChunk(X, Y, Z));
			}
		}*/
		return Success;
	}
	bool space::ForceChunk(int X, int Y, int Z)
	{
		chunk* Chunk = getChunk(X, Y, Z);
		if(Chunk == 0)
		{
			mTerrain[chunkposition(X, Y, Z)] = new chunk;
			mTerrain.find(chunkposition(X, Y, Z))->second->setXPosition(X);
			mTerrain.find(chunkposition(X, Y, Z))->second->setYPosition(Y);
			mTerrain.find(chunkposition(X, Y, Z))->second->setZPosition(Z);
			if(LoadChunk(X, Y, Z))
			{
				return true;
			}
			else
			{
				if(mChunkGenPtr)
				{
					if(mChunkGenPtr->GenerateChunk(mTerrain.find(chunkposition(X, Y, Z))->second))
					{
						CalcChunkAjacencies(chunkposition(X, Y, Z));
						for(std::list<chunkcallback*>::iterator i = mChunkGenCallbacks.begin(); i != mChunkGenCallbacks.end(); i++)
						{
							(*i)->Callback(getChunk(X, Y, Z));
						}
						return true;
					}
					else
					{
						delete getChunk(X, Y, Z);
						mTerrain.erase(mTerrain.find(chunkposition(X, Y, Z)));
						return false;
					}
				}
				else
				{
					delete getChunk(X, Y, Z);
					mTerrain.erase(mTerrain.find(chunkposition(X, Y, Z)));
					return false;
				}
			}
		}
		else
		{
			return true;
		}
	}
	bool space::UnloadChunk(int X, int Y, int Z)
	{
		if(getChunk(X, Y, Z))
		{
			bool SaveSuccess = false;
			if(SaveChunk(X, Y, Z))
			{
				SaveSuccess = true;
			}
			for(std::list<chunkcallback*>::iterator i = mChunkUnloadCallbacks.begin(); i != mChunkUnloadCallbacks.end(); i++)
			{
				(*i)->Callback(getChunk(X, Y, Z));
			}
			delete mTerrain.find(chunkposition(X, Y, Z))->second;
			mTerrain.erase(mTerrain.find(chunkposition(X, Y, Z)));
			return SaveSuccess;
		}
		else
		{
			return false;
		}
	}
	bool space::SaveChunk(int X, int Y, int Z)
	{
		bool Success = false;/*
		std::string Filename = mDirectory;
		
		Filename.append(IntToBase64(X));
		Filename.append(" ");
		Filename.append(IntToBase64(Y));
		Filename.append(" ");
		Filename.append(IntToBase64(Z));
		Filename.append(".wlc");

		datachainnode* Saver = getChunk(X, Y, Z)->getSaver();
		if(Saver != 0)
		{
			Success = SaveDataChainToFile(Filename, Saver);
		}
		getChunk(X, Y, Z)->DoneSaving();
		if(Saver != 0)
		{
			DeleteDataChainStructure(Saver);
		}*/
		return Success;
	}

	void space::SaveAll()
	{
		for(std::map<chunkposition, chunk*>::iterator i = mTerrain.begin(); i != mTerrain.end(); i++)
		{
			SaveChunk(i->first.X, i->first.Y, i->first.Z);
		}
	}
	void space::Clear()
	{
		for(std::map<chunkposition, chunk*>::iterator i = mTerrain.begin(); i != mTerrain.end(); i++)
		{
			for(std::list<chunkcallback*>::iterator i2 = mChunkUnloadCallbacks.begin(); i2 != mChunkUnloadCallbacks.end(); i2++)
			{
				(*i2)->Callback(getChunk((*i).first.X, (*i).first.Y, (*i).first.Z));
			}
			delete i->second;
		}
		mTerrain.clear();
	}
	void space::UnloadAll()
	{
		SaveAll();
		Clear();
	}

	void space::UpdateLoaded()
	{
		if(mTerrain.size() != 0) //Unload all chunks not in loader spheres.
		{
			if(mKeepLoaded.size() != 0)
			{
				std::vector<chunkposition> ToUnload;
				bool ChunkInRange = false;
				int TestSize = mTerrain.size();
				for(std::map<chunkposition, chunk*>::iterator ChunkIter = mTerrain.begin(); ChunkIter != mTerrain.end(); ChunkIter++)
				{
					for(std::list<loadersphere*>::iterator LSIter = mKeepLoaded.begin(); LSIter != mKeepLoaded.end(); LSIter++)
					{
						//ChunkIter->first.X+((real)CHUNK_SIZE/2)
						if(Distance3D(vector3(ChunkIter->first.X+0.5, ChunkIter->first.Y+0.5, ChunkIter->first.Z+0.5),
							(*LSIter)->Position) <= (*LSIter)->Radius)
						{
							ChunkInRange = true;
						}
					}
					if(!ChunkInRange)
					{
						ToUnload.push_back(chunkposition(ChunkIter->first.X, ChunkIter->first.Y, ChunkIter->first.Z));
					}
					ChunkInRange = false;
				}
				for(std::vector<chunkposition>::iterator i = ToUnload.begin(); i != ToUnload.end(); i++)
				{
					UnloadChunk(i->X, i->Y, i->Z);
				}
			}
			else
			{
				UnloadAll();
			}
		}
		if(mKeepLoaded.size() != 0 ) //Load all chunks in loader spheres.
		{
			int StartX, StartY, StartZ;
			int EndX, EndY, EndZ;
			for(std::list<loadersphere*>::iterator LSIter = mKeepLoaded.begin(); LSIter != mKeepLoaded.end(); LSIter++)
			{
				StartX = (*LSIter)->Position.getX() - (int)(*LSIter)->Radius;
				StartY = (*LSIter)->Position.getY() - (int)(*LSIter)->Radius;
				StartZ = (*LSIter)->Position.getZ() - (int)(*LSIter)->Radius;
				EndX = (*LSIter)->Position.getX() + (*LSIter)->Radius;
				EndY = (*LSIter)->Position.getY() + (*LSIter)->Radius;
				EndZ = (*LSIter)->Position.getZ() + (*LSIter)->Radius;
				//std::cout << "StartX: " << StartX << " Start Y: " << StartY << " StartZ: " << StartZ << std::endl;
				//std::cout << "EndX: " << EndX << " End Y: " << EndY << " EndZ: " << EndZ << std::endl;
				for(int XIter = StartX; XIter <= EndX; XIter++)
				{
					for(int YIter = StartY; YIter <= EndY; YIter++)
					{
						for(int ZIter = StartZ; ZIter <= EndZ; ZIter++)
						{
							if(Distance3D(vector3(XIter+0.5, YIter+0.5, ZIter+0.5), (*LSIter)->Position) <= (*LSIter)->Radius)
							{
								ForceChunk(XIter, YIter, ZIter);
							}
						}
					}
				}
			}
		}
	}


	void space::AddLoaderSphere(loadersphere* LS)
	{
		for(std::list<loadersphere*>::iterator i = mKeepLoaded.begin(); i != mKeepLoaded.end(); i++)
		{
			if((*i) == LS)
			{
				return;
			}
		}
		mKeepLoaded.push_back(LS);
	}
	void space::RemoveLoaderSphere(loadersphere* LS)
	{
		for(std::list<loadersphere*>::iterator i = mKeepLoaded.begin(); i != mKeepLoaded.end(); i++)
		{
			if((*i) == LS)
			{
				mKeepLoaded.erase(i);
			}
		}
	}


	void space::AddChunkLoadCallback(chunkcallback* Callback)
	{
		for(std::list<chunkcallback*>::iterator i = mChunkLoadCallbacks.begin(); i != mChunkLoadCallbacks.end(); i++)
		{
			if((*i) == Callback)
			{
				return;
			}
		}
		mChunkLoadCallbacks.push_back(Callback);
	}
	void space::AddChunkUnloadCallback(chunkcallback* Callback)
	{
		for(std::list<chunkcallback*>::iterator i = mChunkUnloadCallbacks.begin(); i != mChunkUnloadCallbacks.end(); i++)
		{
			if((*i) == Callback)
			{
				return;
			}
		}
		mChunkUnloadCallbacks.push_back(Callback);
	}
	void space::AddChunkGenCallback(chunkcallback* Callback)
	{
		for(std::list<chunkcallback*>::iterator i = mChunkGenCallbacks.begin(); i != mChunkGenCallbacks.end(); i++)
		{
			if((*i) == Callback)
			{
				return;
			}
		}
		mChunkGenCallbacks.push_back(Callback);
	}
	void space::RemoveChunkLoadCallback(chunkcallback* Callback)
	{
		if(mChunkLoadCallbacks.size() != 0)
		{
			for(std::list<chunkcallback*>::iterator i = mChunkLoadCallbacks.begin(); i != mChunkLoadCallbacks.end(); i++)
			{
				if((*i) == Callback)
				{
					mChunkLoadCallbacks.erase(i);
					break;
					return;
				}
			}
		}
	}
	void space::RemoveChunkUnloadCallback(chunkcallback* Callback)
	{
		if(mChunkUnloadCallbacks.size() != 0)
		{
			for(std::list<chunkcallback*>::iterator i = mChunkUnloadCallbacks.begin(); i != mChunkUnloadCallbacks.end(); i++)
			{
				if((*i) == Callback)
				{
					mChunkUnloadCallbacks.erase(i);
					break;
					return;
				}
			}
		}
	}
	void space::RemoveChunkGenCallback(chunkcallback* Callback)
	{
		if(mChunkGenCallbacks.size() != 0)
		{
			for(std::list<chunkcallback*>::iterator i = mChunkGenCallbacks.begin(); i != mChunkGenCallbacks.end(); i++)
			{
				if((*i) == Callback)
				{
					mChunkGenCallbacks.erase(i);
					break;
					return;
				}
			}
		}
	}
	
	void space::CalcChunkAjacencies(chunkposition Pos)
	{
		chunk* Chunk = getChunk(Pos.X, Pos.Y, Pos.Z);
		if(Chunk != 0)
		{
			for(int Direction = 0; Direction < 6; Direction++)
			{
				int CheckX = 0;
				int CheckY = 0;
				int CheckZ = 0;
				axis OppositeDirection;

				switch (NumToAxis[Direction])
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
				chunk* OppositeChunk = 0;
				OppositeChunk = getChunk(CheckX+Pos.X, CheckY+Pos.Y, CheckZ+Pos.Z);
				if(OppositeChunk != 0)
				{
					Chunk->registerAjacentChunk(OppositeChunk, NumToAxis[Direction]);
					OppositeChunk->registerAjacentChunk(Chunk, OppositeDirection);
				}
			}
		}
	}
}
