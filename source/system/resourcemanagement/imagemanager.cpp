#include "glresources.h"
#include "SDL_image.h"
namespace CQS
{
StandardImage::~StandardImage()
{
	glDeleteTextures( 1, &Texture );
}

GraphicalResource* CreateStandardImage(SettingBlock& Settings)
{

	StandardImage* TempGR = new StandardImage;
	GLuint TempTex;
	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;
	Uint8 ChromaR, ChromaG, ChromaB;
	ChromaR = 0;
	ChromaG = 0;
	ChromaB = 0;

	bool LoadSuccess = true;

	SDL_Surface * temp = 0;
	bool UseChroma = false;
	std::string Filename("image.bmp");
	if(Settings.GetSettingValueString("Image") != BLANKSTRING)
	{
		Filename = Settings.GetSettingValueString("Image");
	}
	if(Settings.GetSettingValueString("UseChromaKey") != BLANKSTRING)
	{
		if(Settings.GetSettingValueString("UseChromaKey") == "Yes")
		{
			UseChroma = true;
		}
		else if (Settings.GetSettingValueString("UseChromaKey") == "True")
		{
			UseChroma = true;
		}
	}
	if(Settings.GetSettingValueString("ChromaKeyR") != BLANKSTRING)
	{
		ChromaR = atoi(Settings.GetSettingValueString("ChromaKeyR").c_str());
	}
	if(Settings.GetSettingValueString("ChromaKeyG") != BLANKSTRING)
	{
		ChromaG = atoi(Settings.GetSettingValueString("ChromaKeyG").c_str());
	}
	if(Settings.GetSettingValueString("ChromaKeyB") != BLANKSTRING)
	{
		ChromaB = atoi(Settings.GetSettingValueString("ChromaKeyB").c_str());
	}
	 
	/*if ( !(temp = IMG_Load(Filename.c_str())) ) 
	{
		if(!(temp = IMG_Load("error.bmp")))
		{
			Filename.clear();
			Filename.append("error.bmp");
			LoadSuccess = false;
		}
	}*/
	try
	{
		temp = IMG_Load(Filename.c_str());
	}
	catch(...)
	{
	}
	//assert(temp);
	if(LoadSuccess)
	{
		nOfColors = temp->format->BytesPerPixel;
		try
		{
		glGenTextures( 1, &TempTex );
		}
		catch(...)
		{
		}
		if(UseChroma)
		{
			SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(temp->format, ChromaR, ChromaG, ChromaB));
			surface = SDL_DisplayFormatAlpha(temp);
			SDL_FreeSurface( temp );
		}
		else
		{
			surface = SDL_DisplayFormatAlpha(temp);
			SDL_FreeSurface( temp );
		} 
		
		nOfColors = surface->format->BytesPerPixel;
			if (nOfColors == 4)
			{
					if (surface->format->Rmask == 0x000000ff)
					{
						texture_format = GL_RGBA;
					}
					else
					{
						texture_format = GL_BGRA;
					}
			} 
			else if (nOfColors == 3)
			{
					if (surface->format->Rmask == 0x000000ff)
					{
						texture_format = GL_RGB;
					}
					else
					{
						texture_format = GL_BGR;
					}
			}
			glBindTexture( GL_TEXTURE_2D, TempTex );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

			glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
							texture_format, GL_UNSIGNED_BYTE, surface->pixels );
	} 
	else
	{
		Logger TempLogger("error.log");
		std::string Error("SDL could not load image ");
		Error.append(Filename);
		Error.append(" because:");
		TempLogger.LogMessage(Error);
		TempLogger.LogMessage(SDL_GetError());
		return 0;
	}
	if ( surface ) { 
		SDL_FreeSurface( surface );
	}
	TempGR->SetTexture(TempTex);

	return TempGR;
}
}