#ifndef BLOCK_ART_H
#define BLOCK_ART_H

#include "..\..\system\typedefs.h"
#include "..\typedefs.h"

namespace Wanderlust
{
	//Blocks which have varying visuals 

	class chunk;
	class blockart
	{
	public:
		blockart();
		~blockart();

		textureID getTexture(unsigned char, unsigned short, unsigned short, unsigned short); /*For simple blocks, pass side as 
																							  first argument. Last three arguments
																							  are block position.*/
		virtual bool setTexture(unsigned char, textureID); /*For cubes, pass side (as an axis data type) as an argument.
															Returns whether or not you're allowed to do that.*/

		virtual bool isCube(unsigned short, unsigned short, unsigned short); /*Arguments are chunk-space X, Y, and Z position of block.
																			 Tells you that this is the most basic type of 1x1x1 cube block, with
																			 textures in the block texture index (an array texture) and the basic
																			 cube mesh (of which there may be many copies for threading reasons).
																			 Implies that isSimple will return true.*/
		virtual bool isSimple(); //Is this block art type a cube or slope? (I.E. not a unique mesh)
		
		bool getCullsSelf(axis, unsigned short, unsigned short, unsigned short); /*Does this block art obscure vision of
					other instances of itself in this direction? Argument is axis - if you used setTexture on a side of this 
					block art, the same axis value is used to describe whether or not that side is opaque. Only used if 
					isSimple() is true.*/
		bool getCullsOthers(axis, unsigned short, unsigned short, unsigned short); /*Does this block art obscure vision of 
					other block art types in this direction? Argument is axis - if you used setTexture on a side of this block 
					art, the same axis value is used to describe whether or not that side is opaque. Only used if isSimple() 
					is true.*/

		virtual void setCullsSelf(axis, bool);
		virtual void setCullsOthers(axis, bool);

		virtual void setChunkToRender(chunk**); /*Sets the chunk this block art is currently engaged in rendering.
											Points to a pointer (usually to one in the block art index) so that
											changing the current chunk being rendered doesn't require you to
											iterate over every block art and register the new chunk.
										    Create multiple instances of each block art to multithread.*/

		bool Visible(unsigned short, unsigned short, unsigned short);
		void setVisible(bool);
	protected:
		bool mVisible;

		bool mTexturesBlockDependent; /*Does what textures this block art uses, as well as side culling information,
										depend on block-specific information from the chunk?
										If not, we take all of these unsigned short, unsigned short, unsigned short
										xyz positions passed and just ignore them.*/
		textureID* mTextures;
		bool mSideCullSelf[6];
		bool mSideCullOthers[6];
		unsigned short mNumTextures; //How many textures?
		blockart* mSlopedArt;

		chunk** mCurrentChunkRendered;

		//Is this bad programming practice? If so, feel free to punch my face.
		virtual textureID DependentGetTexture(unsigned char, unsigned short, unsigned short, unsigned short){return 0;};
		virtual bool DependentGetCullsSelf(axis, unsigned short, unsigned short, unsigned short){return false;};
		virtual bool DependentGetCullsOthers(axis, unsigned short, unsigned short, unsigned short){return false;};
		virtual bool DependentVisible(unsigned short, unsigned short, unsigned short){return false;};
	};
}

#endif