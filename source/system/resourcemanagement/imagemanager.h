#ifndef GL_RESOURCES_H
#define GL_RESOURCES_H
#include "sprite.h"

#include "SDL.h"
#include "SDL_opengl.h"
#include <GL\gl.h>
namespace CQS
{
class StandardImage : public GraphicalResource
{
public:
	virtual void Draw(Node*, float, float, float, float);
	virtual void SetTexture(GLuint NewTex){Texture = NewTex;};
	~StandardImage();
private:
	GLuint Texture;
};

GraphicalResource* CreateStandardImage(SettingBlock&);
}
#endif