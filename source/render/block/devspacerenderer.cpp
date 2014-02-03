#include "devspacerenderer.h"
#include "..\..\game\world\chunkcallback.h"
#include "..\..\system\typedefs.h"

namespace Wanderlust
{
	void dsrloadcallback::Callback(chunk* C)
	{
		mOurRenderer->AddChunk(C);
	}
	void dsrunloadcallback::Callback(chunk* C)
	{
		mOurRenderer->RemoveChunk(C);
	}

	devspacerenderer::devspacerenderer(space* S, gl_blocktextureindex* TI, blockindex* BI)
	{
		mBlockIndex = BI;
		mTextureIndex = TI;
		mLoadCallback.mOurRenderer = this;
		mUnloadCallback.mOurRenderer = this;
		
		mSpace = S;
		mSpace->AddChunkGenCallback(&mLoadCallback);
		mSpace->AddChunkLoadCallback(&mLoadCallback);
		mSpace->AddChunkUnloadCallback(&mUnloadCallback);
	}
	devspacerenderer::~devspacerenderer()
	{
		mSpace->RemoveChunkGenCallback(&mLoadCallback);
		mSpace->RemoveChunkLoadCallback(&mLoadCallback);
		mSpace->RemoveChunkUnloadCallback(&mUnloadCallback);
		for(std::map<chunk*, chunkblockmodel*>::iterator DrawIter = mToDraw.begin(); DrawIter != mToDraw.end(); DrawIter++)
		{
			delete DrawIter->second;
		}
	}
	
	void devspacerenderer::RebuildChunk(chunk* C)
	{
		if(mToDraw.find(C) != mToDraw.end())
		{
			for(std::list<chunk*>::iterator i = mToRebuild.begin(); i != mToRebuild.end(); i++)
			{
				if(*i == C)
				{
					return;
				}
			}
			mToRebuild.push_back(C);
		}
	}
	void devspacerenderer::_RebuildChunk(chunk* C)
	{
		if(mToDraw.find(C) != mToDraw.end())
		{
			mToDraw.at(C)->Rebuild();
			for(int sideiter = 0; sideiter < 6; sideiter++)
			{
				chunk* Ajacent = C->getAjacentChunk(NumToAxis[sideiter]);
				if(Ajacent != 0)
				{
					if(mToDraw.find(Ajacent) != mToDraw.end())
					{
						axis OppositeDirection;
						switch (NumToAxis[sideiter])
						{
						case POSITIVE_X:
							OppositeDirection = NEGATIVE_X;
							break;
						case NEGATIVE_X:
							OppositeDirection = POSITIVE_X;
							break;
						case POSITIVE_Y:
							OppositeDirection = NEGATIVE_Y;
							break;
						case NEGATIVE_Y:
							OppositeDirection = POSITIVE_Y;
							break;
						case POSITIVE_Z:
							OppositeDirection = NEGATIVE_Z;
							break;
						case NEGATIVE_Z:
							OppositeDirection = POSITIVE_Z;
						}
						mToDraw.find(Ajacent)->second->Rebuild(OppositeDirection);
					}
				}
			}
		}
	}

	void devspacerenderer::AddChunk(chunk* C)
	{
		if(mToDraw.find(C) == mToDraw.end())
		{
			mToDraw[C] = new chunkblockmodel();
			mToDraw.at(C)->regBlockIndex(mBlockIndex);
			mToDraw.at(C)->regChunk(C);
			RebuildChunk(C);
		}
	}
	void devspacerenderer::RemoveChunk(chunk* C)
	{
		if(mToDraw.find(C) != mToDraw.end())
		{
			delete mToDraw.find(C)->second;
			mToDraw.erase(mToDraw.find(C));
		}
	}
	
	
	void devspacerenderer::Draw(glm::mat4 View, glm::mat4 Projection, GLuint ProgramID)
	{
		if(!mToDraw.empty())
		{
			if(!mToRebuild.empty())
			{
				for(std::list<chunk*>::iterator i = mToRebuild.begin(); i != mToRebuild.end(); i++)
				{
					_RebuildChunk(*i);
				}
				mToRebuild.clear();
			}
			GLuint MatrixID = glGetUniformLocation(ProgramID, "MVP");
			for(std::map<chunk*, chunkblockmodel*>::iterator DrawIter = mToDraw.begin(); DrawIter != mToDraw.end(); DrawIter++)
			{
				glm::mat4 Model = glm::translate(glm::mat4(1.0f), 
					glm::vec3(DrawIter->first->getXPosition()*CHUNK_SIZE, DrawIter->first->getYPosition()*CHUNK_SIZE, DrawIter->first->getZPosition()*CHUNK_SIZE));

				glUseProgram(ProgramID);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D_ARRAY, mTextureIndex->getTextureArrayID());

				glm::mat4 MVP = Projection * View * Model;
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
				DrawIter->second->Draw();
			}
		}
	}
}