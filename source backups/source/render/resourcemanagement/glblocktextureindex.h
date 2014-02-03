#ifndef GL_BLOCK_TEXTURE_INDEX_H
#define GL_BLOCK_TEXTURE_INDEX_H

#include "textureindex.h"
#include "..\typedefs.h"

#include <gl/glew.h>
#include <gl/gl.h>
#include <SFML/OpenGL.hpp>
#include <yaml-cpp/yaml.h>
#include <SFML\Graphics.hpp>
#include <vector>
using namespace std;

namespace Wanderlust
{
	//I'm using array textures. If you don't have an OpenGL 3 compatible card/chip, well, fuck you.
	class gl_blocktextureindex : public textureindex
	{
	public:
		gl_blocktextureindex();
		~gl_blocktextureindex();
		virtual void setTextureSize(unsigned short, unsigned short);
		virtual textureID addTexture(string, const void*, size_t); /*Arguments are name, pointer to data, size of data.
																		Returns ID if successful, 0 if not.*/
		//virtual unsigned int getTextureIDByName(string);
		//virtual unsigned int getTextureCount();
		virtual void Clear();
		virtual void Build(); //Creates the OpenGL array texture from all currently registered textures.
		virtual void Rebuild(); //Destroys the OpenGL array texture and then builds it again.
		virtual void setInGame(bool); /*Are we no longer loading? Should we rebuild every time a texture is added?*/
		virtual bool getIsInGame();

		virtual GLuint getTextureArrayID();
	protected:
		bool mIsInGame;
		unsigned short mTexWidth, mTexHeight;
		vector<sf::Image> mImages;
		unsigned char* mImageData; //Image data in uint8 RGBA.
		GLuint mTexArrayID; 
	};
}

#endif