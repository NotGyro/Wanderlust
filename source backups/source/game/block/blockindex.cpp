#include "blockindex.h"

namespace Wanderlust
{
	blockID blockindex::addBlock(std::string Name)
	{	
		if(mIndexedByName.find(Name) == mIndexedByName.end())
		{
			mBlocks.push_back(block());
			mIndexedByName[Name] = mBlocks.size() - 1;
			mIndexedByID[mBlocks.size() - 1] = Name;
			return mBlocks.size() - 1; //Block ID
		}
		else
		{
			return 0;
		}
	}

	block* blockindex::getBlockByID(blockID ToGet)
	{
		if((ToGet < mBlocks.size()) && (ToGet != 0))
		{
			return &mBlocks.at(ToGet);
		}
		return 0;
	}
	block* blockindex::getBlockByName(std::string Name)
	{
		if(mIndexedByName.find(Name) != mIndexedByName.end())
		{
			return getBlockByID(mIndexedByName.find(Name)->second);
		}
		else
		{
			return 0;
		}
	}
	blockID blockindex::getBlockIDByName(std::string Name)
	{
		if(mIndexedByName.find(Name) != mIndexedByName.end())
		{
			return mIndexedByName.find(Name)->second;
		}
		else
		{
			return 0;
		}
	}
	std::string blockindex::getNameByBlockID(blockID ID)
	{
		if(mIndexedByID.find(ID) != mIndexedByID.end())
		{
			return mIndexedByID.find(ID)->second;
		}
		else
		{
			return std::string();
		}
	}

	unsigned short blockindex::getBlockCount()
	{
		return mBlocks.size();
	}
}