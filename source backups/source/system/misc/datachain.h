#ifndef TERRIBLE_CODE_H
#define TERRIBLE_CODE_H

#include <string>

namespace Wanderlust
{
	struct datachainheader
	{
		unsigned char Depth;
		unsigned long DataSize;
		unsigned char ExtraSize;
		char* HeaderData;
		bool operator==(datachainheader&);
	};

	struct datachainnode
	{
		datachainheader Header;
		datachainnode* Next;
		char* Data;
	};

	void DeleteDataChainStructure(datachainnode*); /*Deletes the structure of a data chain, not the data it points to.*/
	bool LoadDataChainDataFromFile(std::string, datachainnode*);
	datachainnode* LoadDataChainStructureFromFile(std::string);
	bool SaveDataChainToFile(std::string, datachainnode*);
}
#endif