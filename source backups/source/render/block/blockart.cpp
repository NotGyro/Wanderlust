#include "blockart.h"

namespace Wanderlust
{
	blockart::blockart()
	{
		mTexturesBlockDependent = false; /*ALWAYS ALWAYS ALWAYS put this first in every block art constructor.
										 Unless the textures are block-dependant in which case... yeah, obvious
										 stuff.*/
		mNumTextures = 6;
		mTextures = new textureID[mNumTextures];
		for(int i = 0; i < mNumTextures; i++)
		{
			mTextures[i] = 0;
			mSideCullSelf[i] = true;
			mSideCullOthers[i] = true;
		}

		mCurrentChunkRendered = 0;
		mVisible = true;
	}
	blockart::~blockart()
	{
		delete [] mTextures;
	}
	bool blockart::Visible(unsigned short x, unsigned short y, unsigned short z)
	{
		if(mTexturesBlockDependent)
		{
			return DependentVisible(x, y, z);
		}
		else
		{
			return mVisible;
		}
	}
	void blockart::setVisible(bool V)
	{
		mVisible = V;
	}

	textureID blockart::getTexture(unsigned char TexIndex, unsigned short x, unsigned short y, unsigned short z)
	{
		if(mTexturesBlockDependent)
		{
			return DependentGetTexture(TexIndex, x, y, z);
		}
		else
		{
			if(TexIndex < mNumTextures)
			{
				return mTextures[TexIndex];
			}
			else
			{
				return 0;
			}
		}
	}
	bool blockart::setTexture(unsigned char TexIndex, textureID SetTo)
	{
		if(TexIndex < mNumTextures)
		{
			mTextures[TexIndex] = SetTo;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool blockart::isCube(unsigned short x, unsigned short y, unsigned short z)
	{
		return true;
	}
	bool blockart::isSimple()
	{
		return true;
	}
		
	bool blockart::getCullsSelf(axis Direction, unsigned short x, unsigned short y, unsigned short z)
	{
		if(mTexturesBlockDependent)
		{
			return DependentGetTexture(Direction, x, y, z);
		}
		else
		{
			return mSideCullSelf[Direction];
		}
	}
	bool blockart::getCullsOthers(axis Direction, unsigned short x, unsigned short y, unsigned short z)
	{
		if(mTexturesBlockDependent)
		{
			return DependentGetTexture(Direction, x, y, z);
		}
		else
		{
			return mSideCullOthers[Direction];
		}
	}

	void blockart::setCullsSelf(axis Direction, bool SetTo)
	{
		mSideCullSelf[Direction] = SetTo;
	}
	void blockart::setCullsOthers(axis Direction, bool SetTo)
	{
		mSideCullOthers[Direction] = SetTo;
	}

	void blockart::setChunkToRender(chunk** Chunk)
	{
		mCurrentChunkRendered = Chunk;
	}
}