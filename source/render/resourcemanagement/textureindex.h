#ifndef TEXTURE_INDEX_H
#define TEXTURE_INDEX_H

#include <map>
#include <string>
#include "..\typedefs.h"
using namespace std;

namespace Wanderlust
{
	class textureindex
	{
	public:
		textureindex();
		virtual textureID addTexture(string, const void*, size_t) {return 0;}; /*Arguments are name, pointer to data, size of data.
																		Returns ID if successful, 0 if not.*/
		virtual textureID getTextureIDByName(string);
		virtual unsigned int getTextureCount();
		virtual void Clear(){};
	protected:
		map<string, textureID> mNameToID;
		unsigned int mTextureCount;
	};
}

#endif