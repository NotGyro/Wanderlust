#ifndef BLOCK_LAYER_BYTE_H
#define BLOCK_LAYER_BYTE_H

#include "..\constants.h"
#include "..\..\..\system\typedefs.h"

#include "blocklayerabstract.h"

namespace Wanderlust
{
	//class chunkmodel;
	//class chunkblockmodel;
	class byteblocklayer : public blocklayer
	{
	public:
		byteblocklayer();
		~byteblocklayer();

		byte getBlock(unsigned short, unsigned short, unsigned short); //X, Y, Z
		void setBlock(unsigned short, unsigned short, unsigned short, byte); //X, Y, Z, data.

		virtual void InitToZero();

		virtual byte* getData();
		virtual size_t getDataSize();
	protected:
		/*Fun with stacked arrays: Holy shit how do I explain this without a picture.
	
		A stacked array is a one-dimensional array that acts like a multidimensional array.
		An element is accessed by multiplying the desired Z position by the width times the height
		of the array, adding the desired Y position multiplied by the width of the array, and
		then adding the desired X position. In this way, a one-dimensional array can be made
		to act like it has multiple dimensions.

		Suppose you have a stacked array that is three by three by three. The third element,
		which is to say, stacked[2], will be the (2, 0, 0) element. However, once you
		go past that - the fourth element, stacked[3], would actually be (0, 1, 0).
		Similarly, the tenth element, stacked[9], would be (0, 0, 1), and stacked[10]
		would be (1, 0 1).

		I can't explain it any better than that at the moment.
		
		So, here you have a rope that is 32*32*32 long which acts
		like a cube that is 32 by 32 by 32.*/
		byte mBlockData[(CHUNK_SIZE * CHUNK_SIZE) * CHUNK_SIZE];
	};
}

#endif