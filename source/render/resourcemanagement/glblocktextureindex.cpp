#include "glblocktextureindex.h"

namespace Wanderlust
{
	gl_blocktextureindex::gl_blocktextureindex()
	{
		mTextureCount = 0;
		mTexArrayID = 0;
		mImageData = 0;
		mIsInGame = false;
		mTexWidth = 0;
		mTexHeight = 0;
	}
	gl_blocktextureindex::~gl_blocktextureindex()
	{
		Clear();
	}
	void gl_blocktextureindex::setTextureSize(unsigned short x, unsigned short y)
	{
		assert(!mIsInGame); //God, the shit that could happen if we alter the texture size while in a game like that.
		mTexWidth = x;
		mTexHeight = y;
	}
	textureID gl_blocktextureindex::addTexture(string Name, const void* Data, size_t DataSize)
	{
		mImages.push_back(sf::Image());
		if(mImages.back().loadFromMemory(Data, DataSize))
		{
			if(mNameToID.find(Name) == mNameToID.end())
			{
				mTextureCount++;
				mNameToID[Name] = mTextureCount;
				if(getIsInGame())
				{
					Rebuild();
				}
				return mTextureCount;
			}
		}
		mImages.pop_back(); //Any failure condition ends up here.
		return 0;		
	}
	void gl_blocktextureindex::Clear()
	{
		mImages.clear();
		mTextureCount = 0;
		if(mImageData != 0)
		{
			delete [] mImageData;
			mImageData = 0;
		}
		if(mTexArrayID != 0)
		{
			glDeleteTextures(1, &mTexArrayID);
			mTexArrayID = 0;
		}
	}
	void gl_blocktextureindex::Build()
	{
		//Create the image data container that will be used to load our array texture.
		unsigned int DataSize = mTextureCount * ((mTexWidth*mTexHeight)/*Pixels in image*/ * 4 /*bytes per pixel*/);
		mImageData = new unsigned char[DataSize];
		unsigned char* ImageDataIter = mImageData; //For writing to mImageData
		const unsigned char* ReadFrom; //for reading from sf::Image data.
		//unsigned short ReadOffset;
		/*Go through each image in order and copy all of the pixel values to the mImageData buffer.
		Each image should end up as a layer in the texture array.*/
		for(vector<sf::Image>::iterator Iter = mImages.begin(); Iter != mImages.end(); Iter++)
		{
			ReadFrom = Iter->getPixelsPtr();
			assert(Iter->getSize().x == mTexWidth);
			assert(Iter->getSize().y == mTexHeight);
			if(ImageDataIter < (mImageData + DataSize)) //Only you can prevent buffer overflows.
			{
				memcpy(static_cast<void*>(ImageDataIter), ReadFrom, (mTexWidth*mTexHeight)/*Pixels in image*/ * 4 /*bytes per pixel*/);
				ImageDataIter += (mTexWidth*mTexHeight)/*Pixels in image*/ * 4 /*bytes per pixel*/;
			}
			else
			{
				break;
			}
		}
		
		glGenTextures(1, &mTexArrayID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, mTexArrayID);

		glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Filtering stuff.
		glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); //Wrap properly.
		glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, mTexWidth, mTexHeight, mTextureCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImageData);
		
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}
	void gl_blocktextureindex::Rebuild()
	{
		if(mImageData != 0)
		{
			delete [] mImageData;
		}
		if(mTexArrayID != 0)
		{
			glDeleteTextures(1, &mTexArrayID);
		}
		Build();
	}
	void gl_blocktextureindex::setInGame(bool B)
	{
		mIsInGame = B;
	}
	bool gl_blocktextureindex::getIsInGame()
	{
		return mIsInGame;
	}
	GLuint gl_blocktextureindex::getTextureArrayID()
	{
		return mTexArrayID;
	}
}