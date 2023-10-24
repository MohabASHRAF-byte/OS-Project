/*
 * dynamic_allocator.c
 *
 *  Created on: Sep 21, 2023
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"


//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//=====================================================
// 1) GET BLOCK SIZE (including size of its meta data):
//=====================================================
uint32 get_block_size(void* va)
{
	struct BlockMetaData *curBlkMetaData = ((struct BlockMetaData *)va - 1) ;
	return curBlkMetaData->size ;
}

//===========================
// 2) GET BLOCK STATUS:
//===========================
int8 is_free_block(void* va)
{
	struct BlockMetaData *curBlkMetaData = ((struct BlockMetaData *)va - 1) ;
	return curBlkMetaData->is_free ;
}

//===========================================
// 3) ALLOCATE BLOCK BASED ON GIVEN STRATEGY:
//===========================================
void *alloc_block(uint32 size, int ALLOC_STRATEGY)
{
	void *va = NULL;
	switch (ALLOC_STRATEGY)
	{
	case DA_FF:
		va = alloc_block_FF(size);
		break;
	case DA_NF:
		va = alloc_block_NF(size);
		break;
	case DA_BF:
		va = alloc_block_BF(size);
		break;
	case DA_WF:
		va = alloc_block_WF(size);
		break;
	default:
		cprintf("Invalid allocation strategy\n");
		break;
	}
	return va;
}

//===========================
// 4) PRINT BLOCKS LIST:
//===========================

void print_blocks_list(struct MemBlock_LIST list)
{
	cprintf("=========================================\n");
	struct BlockMetaData* blk ;
	cprintf("\nDynAlloc Blocks List:\n");
	LIST_FOREACH(blk, &list)
	{
		cprintf("(size: %d, isFree: %d)\n", blk->size, blk->is_free) ;
	}
	cprintf("=========================================\n");

}
//
////********************************************************************************//
////********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//==================================
// [1] INITIALIZE DYNAMIC ALLOCATOR:
//==================================
void initialize_dynamic_allocator(uint32 daStart, uint32 initSizeOfAllocatedSpace)
{
	//=========================================
	//DON'T CHANGE THESE LINES=================
	if (initSizeOfAllocatedSpace == 0)
		return ;
	//=========================================
	//=========================================

	//TODO: [PROJECT'23.MS1 - #5] [3] DYNAMIC ALLOCATOR - initialize_dynamic_allocator()
	struct BlockMetaData* blk  = (struct BlockMetaData *) daStart;


	blk->size =  initSizeOfAllocatedSpace ;
	blk->is_free = 1  ;

//	LIST_HEAD(blk , BlockMetaData);
	LIST_INSERT_HEAD(&mem_blocks, blk);


	//panic("initialize_dynamic_allocator is not implemented yet");
}

//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
void *alloc_block_FF(uint32 size)
{
	//TODO: [PROJECT'23.MS1 - #6] [3] DYNAMIC ALLOCATOR - alloc_block_FF()
	//panic("alloc_block_FF is not implemented yet");
	if(size == 0){
		return NULL;
	}

	struct BlockMetaData* block;
	uint32 total_size = size + sizeOfMetaData();
	LIST_FOREACH(block, &mem_blocks){
		if(block->size == total_size && block->is_free){
			uint32 start = (uint32)(block);
			block->is_free = 0;
			return (void *)(start + sizeOfMetaData());
		} else if((block->size - sizeOfMetaData()) >= total_size && block->is_free){
			uint32 start = (uint32)(block);

			struct BlockMetaData* new_block = (struct BlockMetaData *)((uint32)block + total_size);
			new_block->size = block->size - total_size;
			new_block->is_free = 1;
			block->is_free = 0;
			block->size = total_size;
			LIST_INSERT_AFTER(&mem_blocks, block, new_block);

			return (void *)(start + sizeOfMetaData());
		}
	}

	uint32* tmp = (uint32 *)sbrk(total_size);

	if(tmp == (uint32 *)-1){
		return NULL;
	} else {
		struct BlockMetaData* new_block = (struct BlockMetaData *)((uint32)tmp);
		new_block->size = total_size;
		new_block->is_free = 0;
		LIST_INSERT_TAIL(&mem_blocks, new_block);

		return tmp;
	}

	return NULL;
}
//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
void *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT'23.MS1 - BONUS] [3] DYNAMIC ALLOCATOR - alloc_block_BF()
	panic("alloc_block_BF is not implemented yet");
	return NULL;
}

//=========================================
// [6] ALLOCATE BLOCK BY WORST FIT:
//=========================================
void *alloc_block_WF(uint32 size)
{
	panic("alloc_block_WF is not implemented yet");
	return NULL;
}

//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
void *alloc_block_NF(uint32 size)
{
	panic("alloc_block_NF is not implemented yet");
	return NULL;
}

//===================================================
// [8] FREE BLOCK WITH COALESCING:
//===================================================
void free_block(void *va)
{
	//TODO: [PROJECT'23.MS1 - #7] [3] DYNAMIC ALLOCATOR - free_block()
//	panic("free_block is not implemented yet");
	if(va==0)
		return;
	struct BlockMetaData *curBlkMetaData = ((struct BlockMetaData *)va - 1) ;
	curBlkMetaData->is_free=1;
	struct BlockMetaData *next = LIST_NEXT(curBlkMetaData);
	if(next != NULL && next->is_free)
	{
		curBlkMetaData->size+=next->size;
		next->size=0;
		next->is_free=0;
	}
	struct BlockMetaData *prev = LIST_PREV(curBlkMetaData);
	if(prev != NULL && prev->is_free)
	{
		prev->size+=curBlkMetaData->size;
		curBlkMetaData->size=0;
		curBlkMetaData->is_free=0;
	}
	return;
}

//=========================================
// [4] REALLOCATE BLOCK BY FIRST FIT:
//=========================================
void *realloc_block_FF(void* va, uint32 new_size)
{
	//TODO: [PROJECT'23.MS1 - #8] [3] DYNAMIC ALLOCATOR - realloc_block_FF()
	panic("realloc_block_FF is not implemented yet");
	return NULL;
}
