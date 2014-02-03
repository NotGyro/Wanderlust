#include <fstream>
using namespace std;
#include "datachain.h"

namespace Wanderlust
{
	bool datachainheader::operator==(datachainheader& CompareTo)
	{
		return DataSize == CompareTo.DataSize;
	}
	void DeleteDataChainStructure(datachainnode* DCN)
	{
		datachainnode* Current = DCN;
		datachainnode* Previous;
		while(Current->Next != 0)
		{
			Previous = Current;
			Current = Current->Next;
			delete Previous->Header.HeaderData;
			delete Previous;
		}
	}
	bool LoadDataChainDataFromFile(std::string Filename, datachainnode* FirstNode)
	{
		ifstream Load (Filename, ios::in|ios::binary|ios::ate);
		ifstream::pos_type size;
		datachainheader TempHeader;
		datachainnode* Current = FirstNode; 
		unsigned long GetPosition = 0;
		if (Load.is_open())
		{
			size = Load.tellg();
			while(GetPosition <= size)
			{
				Load.seekg (GetPosition, ios::beg);
				Load.read ((char*)&TempHeader.Depth, sizeof(TempHeader.Depth));
				GetPosition += sizeof(TempHeader.Depth);

				Load.seekg (GetPosition, ios::beg);
				Load.read ((char*)&TempHeader.ExtraSize, sizeof(TempHeader.ExtraSize)); //Read the size of additonal header data.
				GetPosition += sizeof(TempHeader.ExtraSize);
				
				if(TempHeader.ExtraSize > 0)
				{
					Load.seekg (GetPosition, ios::beg);
					TempHeader.HeaderData = new char[TempHeader.ExtraSize];
					Load.read (TempHeader.HeaderData, TempHeader.ExtraSize); //Read the additonal header data.
					GetPosition += TempHeader.ExtraSize;
				}
				else
				{
					TempHeader.HeaderData = 0;
				}
				
				Load.seekg (GetPosition, ios::beg);
				Load.read ((char*)&TempHeader.DataSize, sizeof(TempHeader.DataSize)); //Read the data size.
				GetPosition += sizeof(TempHeader.DataSize);

				Load.seekg (GetPosition, ios::beg);
				if(TempHeader == Current->Header)
				{
					if((TempHeader.DataSize > 0) && (Current->Data != 0))
					{
						Load.read(Current->Data, TempHeader.DataSize);
						GetPosition += TempHeader.DataSize;
					}
					Current = Current->Next;
				}
				else
				{
					return false;
				}
				delete TempHeader.HeaderData;
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	datachainnode* LoadDataChainStructureFromFile(std::string Filename)
	{
		ifstream Load (Filename, ios::in|ios::binary|ios::ate);
		ifstream::pos_type size;

		datachainnode* First = new datachainnode; 
		datachainnode* Current = First;

		unsigned long GetPosition = 0;
		if (Load.is_open())
		{
			size = Load.tellg();
			while(GetPosition <= size)
			{
				Load.seekg (GetPosition, ios::beg);
				Load.read ((char*)&Current->Header.Depth, sizeof(Current->Header.Depth));
				GetPosition += sizeof(Current->Header.Depth);

				Load.seekg (GetPosition, ios::beg);
				Load.read ((char*)&Current->Header.ExtraSize, sizeof(Current->Header.ExtraSize)); //Read the size of additonal header data.
				GetPosition += sizeof(Current->Header.ExtraSize);
				
				if(Current->Header.ExtraSize > 0)
				{
					Load.seekg (GetPosition, ios::beg);
					Current->Header.HeaderData = new char[Current->Header.ExtraSize];
					Load.read (Current->Header.HeaderData, Current->Header.ExtraSize); //Read the additonal header data.
					GetPosition += Current->Header.ExtraSize;
				}
				else
				{
					Current->Header.HeaderData = 0;
				}
				
				Load.seekg (GetPosition, ios::beg);
				Load.read ((char*)&Current->Header.DataSize, sizeof(Current->Header.DataSize)); //Read the data size.
				GetPosition += sizeof(Current->Header.DataSize);

				GetPosition += Current->Header.DataSize;
				Current->Next = new datachainnode;
				Current = Current->Next;
			}
			return First;
			Load.close();
		}
		else
		{
			return 0;
		}
	}
	bool SaveDataChainToFile(std::string Filename, datachainnode* SaveThis)
	{
		datachainnode* Current = SaveThis;

		ofstream Save (Filename, ios::out|ios::binary|ios::trunc);
		if(Save.is_open())
		{
			unsigned long SavePos = 0;
			while(Current != 0)
			{
				Save.seekp(SavePos);
				Save.write((char*)&Current->Header.Depth, sizeof(Current->Header.Depth));
				SavePos += sizeof(Current->Header.Depth);

				Save.seekp(SavePos);
				Save.write((char*)&Current->Header.ExtraSize, sizeof(Current->Header.ExtraSize));
				SavePos += sizeof(Current->Header.ExtraSize);

				if(Current->Header.ExtraSize > 0)
				{
					Save.seekp(SavePos, ios::beg);
					Save.write (Current->Header.HeaderData, Current->Header.ExtraSize);
					SavePos += Current->Header.ExtraSize;
				}
				
				Save.seekp(SavePos, ios::beg);
				Save.write ((char*)&Current->Header.DataSize, sizeof(Current->Header.DataSize)); //Read the data size.
				SavePos += sizeof(Current->Header.DataSize);
			
				if((Current->Header.DataSize > 0) && (Current->Data != 0))
				{
					Save.seekp(SavePos, ios::beg);
					Save.write(Current->Data, Current->Header.DataSize);
					SavePos += Current->Header.DataSize;
				}
				Current = Current->Next;
			}
			Save.close();		
			return true;
		}
		else
		{
			return false;
		}
	}
}