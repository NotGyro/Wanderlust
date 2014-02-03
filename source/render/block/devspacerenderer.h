#ifndef SPACE_RENDERER_H
#define SPACE_RENDERER_H
//A class that will only exist for a little while, probably. 
#include "..\..\game\world\space.h"
#include "chunkblockmodel.h"
#include "..\resourcemanagement\glblocktextureindex.h"
#include "..\..\game\block\blockindex.h"
#include "blockart.h"
#include <map>
#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Wanderlust
{
	class devspacerenderer;
	class dsrloadcallback : public chunkcallback
	{
	public:
		devspacerenderer* mOurRenderer;
		virtual void Callback(chunk* C);
	};
	class dsrunloadcallback : public chunkcallback
	{
	public:
		devspacerenderer* mOurRenderer;
		virtual void Callback(chunk* C);
	};
	class devspacerenderer
	{
	public:
		devspacerenderer(space*, gl_blocktextureindex*, blockindex*);
		~devspacerenderer();
		virtual void RebuildChunk(chunk*);
		virtual void AddChunk(chunk*);
		virtual void RemoveChunk(chunk*);

		virtual void Draw(glm::mat4, glm::mat4, GLuint);
	protected:
		std::map<chunk*, chunkblockmodel*> mToDraw;
		std::list<chunk*> mToRebuild;
		virtual void _RebuildChunk(chunk*);
		gl_blocktextureindex* mTextureIndex;
		blockindex* mBlockIndex;
		space* mSpace;

		dsrloadcallback mLoadCallback;
		dsrunloadcallback mUnloadCallback;
	};
}
#endif