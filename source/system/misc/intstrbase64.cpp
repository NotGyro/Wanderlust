#include "intstrbase64.h"
#include <math.h>
#include <algorithm>
#include <string>
#include <map>

#include "..\math\typedefs.h"

namespace Wanderlust
{
	static const char Table[64] =
	{
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'i',
		'j',
		'k',
		'l',
		'm',
		'n',
		'o',
		'p',
		'q',
		'r',
		's',
		't',
		'u',
		'v',
		'w',
		'x',
		'y',
		'z',
		'A',
		'B',
		'C',
		'D',
		'E',
		'F',
		'G',
		'H',
		'I',
		'J',
		'K',
		'L',
		'M',
		'N',
		'O',
		'P',
		'Q',
		'R',
		'S',
		'T',
		'U',
		'V',
		'W',
		'X',
		'Y',
		'Z',
		'•',
		'‡'
	};
	std::map<char, short int> ReverseTable;
	std::string IntToBase64(int ToParse)
	{
		std::string Output;
		Output.clear();
		unsigned int UToParse;

		char sign;
		if(ToParse >= 0)
		{
			UToParse = ToParse;
			sign = '+';
		}
		else
		{
			UToParse = ToParse * -1;
			sign = '-';
		}
		if(ToParse < 64)
		{
			Output.push_back(Table[UToParse]);
		}
		else
		{
			//Output.push_back(Table[UToParse % 64]);
			for(int PowOf64 = 0; pow(real(64), PowOf64) <= ToParse; PowOf64++)
			{
				real Check1 = floor(real(UToParse)/pow(real(64), PowOf64));
				int Check2 = int(Check1) % 64;
				Output.push_back(Table[Check2]);
			}
		}
		Output.push_back(sign);
		std::reverse<std::string::iterator>(Output.begin(), Output.end());
		return Output;
	}
	int Base64ToInt(std::string ToParse)
	{
		if(ReverseTable.empty())
		{
			for(int i = 0; i < 64; i++)
			{
				ReverseTable[Table[i]] = i;
			}
		}
		int sign = 0;
		if((*ToParse.begin()) == '+')
		{
			sign = 1;
		}
		else if((*ToParse.begin()) == '-')
		{
			sign = -1;
		}
		std::reverse<std::string::iterator>(ToParse.begin(), ToParse.end());
		if(ToParse.size() <= 2)
		{
			return ReverseTable.find(*ToParse.begin())->second * sign;
		}
		else
		{
			int returnvalue = 0;
			int PowOf64 = 0;
			std::string::iterator End = ToParse.end();
			End--;
			for(std::string::iterator Iter = ToParse.begin(); Iter != End; Iter++)
			{
				returnvalue += (ReverseTable.find(*Iter)->second) * pow(real(64), PowOf64);
				PowOf64++;
			}
			return returnvalue * sign;
		}
		return 0;
	}
}