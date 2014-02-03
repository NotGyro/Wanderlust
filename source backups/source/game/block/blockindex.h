#ifndef BLOCK_INDEX_H
#define BLOCK_INDEX_H

#include "block.h"
#include "..\world\constants.h"

#include <map>
#include <vector>
#include <string>

namespace Wanderlust
{
	class blockindex
	{
	public:
		//Default constructor/destructor will probably be good enough?
		//blockindex();
		//~blockindex();
		
		blockID addBlock(std::string); //Takes name as argument, creates default block, returns its ID.

		block* getBlockByID(blockID);
		block* getBlockByName(std::string);
		blockID getBlockIDByName(std::string);
		std::string getNameByBlockID(blockID);
		unsigned short getBlockCount();
	protected:
		std::vector<block> mBlocks; /*Only insert elements with push_back and delete elements by deleting
									the entire block index. Otherwise, you will invalidate a whole hell of
									a lot of pointers.*/
		std::map<std::string, blockID> mIndexedByName;
		std::map<blockID, std::string> mIndexedByID;
	};
}

#endif