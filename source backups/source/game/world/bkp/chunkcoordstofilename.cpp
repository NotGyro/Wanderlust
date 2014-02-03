#include "chunkcoordstofilename.h"
#include <math.h>
namespace Wanderlust
{
	//This is fucked.
	//instead, make a base 32 system off of 0-9 and the first 23 lowercase letters of the English alphabet.
	//Uppercase X, Y, Z
	//Oh wait, that would be uppercase X or Z
	//denote the next coordinate, and the letter
	//immediately after it denotes the sign, i.e. Xp, Xn.
	//(9, -20, 33) would look like this (if we were using 3 dimensions):
	//Xp9YnlZp11
	//(-20, 63) would look like this:
	//XnlZp1w
	//(-20, 64) would look like this:
	//XnlZp20
	/*string chunkcoordstofilename(int X, int Y, int Z)
	{
		unsigned int uX = (unsigned int)abs(X);
		unsigned int uY = (unsigned int)abs(Y);
		unsigned int uZ = (unsigned int)abs(Z);

		unsigned char XInChars[sizeof(unsigned int)];
		for(int iter = 0; iter < sizeof(unsigned int); iter++)
		{
			XInChars[iter] = (*((unsigned char*)&uX)+iter);
		}

		unsigned char YInChars[sizeof(unsigned int)];
		for(int iter = 0; iter < sizeof(unsigned int); iter++)
		{
			YInChars[iter] = (*((unsigned char*)&uY)+iter);
		}

		unsigned char ZInChars[sizeof(unsigned int)];
		for(int iter = 0; iter < sizeof(unsigned int); iter++)
		{
			ZInChars[iter] = (*((unsigned char*)&uZ)+iter);
		}

		//X coordinate grouping is sizeof(int)+1 characters
		std::string Result;
		if(X < 0)
		{
			Result.push_back('-');
		}
		else
		{
			Result.push_back('+');
		}
		for(int iter = 0; iter < sizeof(unsigned int); iter++)
		{
			Result.push_back((char)XInChars[iter]);
		}

		if(Y < 0)
		{
			Result.push_back('-');
		}
		else
		{
			Result.push_back('+');
		}
		for(int iter = 0; iter < sizeof(unsigned int); iter++)
		{
			Result.push_back((char)YInChars[iter]);
		}

		if(Z < 0)
		{
			Result.push_back('-');
		}
		else
		{
			Result.push_back('+');
		}
		for(int iter = 0; iter < sizeof(unsigned int); iter++)
		{
			Result.push_back((char)ZInChars[iter]);
		}
		return Result;
	}*/
	string chunkcoordstofilename(int X, int Y, int Z)
	{
		unsigned int uX = (unsigned int)abs(X);
		unsigned int uY = (unsigned int)abs(Y);
		unsigned int uZ = (unsigned int)abs(Z);

		unsigned int PowOf32 = 1; //32 ^ 0;

		while(PowOf32 <= uX)
		{
			PowOf32 *= 32;
		}
	}
	std::tuple<int, int, int> chunkcoordsfromfilename(std::string Name)
	{
	}
}