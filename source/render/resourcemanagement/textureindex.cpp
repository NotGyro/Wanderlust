#include "textureindex.h"

namespace Wanderlust
{
	textureindex::textureindex()
	{
		mTextureCount = 0;
	}

	textureID textureindex::getTextureIDByName(string Name)
	{
		if(mNameToID.find(Name) != mNameToID.end())
		{
			return mNameToID.find(Name)->second;
		}
		else
		{
			return 0;
		}
	}

	unsigned int textureindex::getTextureCount()
	{
		return mTextureCount;
	}
}