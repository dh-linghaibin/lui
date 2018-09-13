/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "sram.h"

#include <string.h>
#include <stdio.h>


#define SRAM_SIZE (100000)

#define HeapBlockSIZE (100)

//#define SRAM_BASE_ADDR 0
static char SRAM_BASE_ADDR[SRAM_SIZE] = {0};

typedef struct A_BLOCK_LINK
{
    char *pV;
    int  index;
    char empty;
    int  heapLen;
    struct A_BLOCK_LINK *pxNextFreeBlock;
    struct A_BLOCK_LINK *pxPrevFreeBlock;
} BlockLink_t;


static char *HeapHead = NULL;

static int BlockTotal = 0;

void trace_heap(void)
{
    BlockLink_t *pTempBlockLink = (BlockLink_t *)HeapHead;
    debug_pt("\r\n##########trace_heap#############\r\n");
    while(pTempBlockLink)
    {
        debug_pt("index: %d empty:%d addr:%d \r\n", pTempBlockLink->index, pTempBlockLink->empty, pTempBlockLink->pV - SRAM_BASE_ADDR);
        pTempBlockLink = pTempBlockLink->pxNextFreeBlock;
    }
    debug_pt("\r\n##########trace_heap#############\r\n");
}


void init_heap(void)
{
    BlockLink_t *pBlockLink, *pTempBlockLink = NULL;
    int i = 0;
    char *pHeapStart = (char *)SRAM_BASE_ADDR;
    char *pHeapEnd   = (char *)(SRAM_BASE_ADDR + SRAM_SIZE);
    pHeapEnd -= HeapBlockSIZE;//back 
    BlockTotal = SRAM_SIZE / (HeapBlockSIZE + sizeof(BlockLink_t));
    while(i < BlockTotal)
    {
        pBlockLink          = (BlockLink_t *)pHeapStart;
        pBlockLink->pxPrevFreeBlock = pTempBlockLink;
        pBlockLink->pV      = pHeapEnd;
        pBlockLink->index   = i;
        pBlockLink->empty   = 1;
        pBlockLink->heapLen = 0;

        pHeapEnd   -= HeapBlockSIZE;
        pHeapStart += sizeof(BlockLink_t);

        pTempBlockLink = pBlockLink;
        pBlockLink->pxNextFreeBlock = (BlockLink_t *)pHeapStart;    
        i++;
    }
    pBlockLink->pxNextFreeBlock = NULL;
    HeapHead = (char *)SRAM_BASE_ADDR;
}

static BlockLink_t *find_heap(char *addr)
{
    BlockLink_t *pTempBlockLink = (BlockLink_t *)HeapHead;

    while(pTempBlockLink)
    {
        if(pTempBlockLink->pV == addr)
        {
            return pTempBlockLink;
        }

        pTempBlockLink = pTempBlockLink->pxNextFreeBlock;
    }
    return NULL;
}

static void *Sramfind_heap(int size)
{
    char *mem;
    int seriesSize = 0;
    BlockLink_t *pTempBlockLink;
    pTempBlockLink = (BlockLink_t *)HeapHead;

    while(pTempBlockLink)
    {
        if(pTempBlockLink->empty)
        {
            seriesSize += HeapBlockSIZE;
        }
        else
        {
            seriesSize = 0;
        }
        if(seriesSize >= size)
        {    
            pTempBlockLink->heapLen = seriesSize;
            mem = pTempBlockLink->pV;

            while(seriesSize && pTempBlockLink)
            {
                seriesSize -= HeapBlockSIZE;
                pTempBlockLink->empty = 0;
                pTempBlockLink = pTempBlockLink->pxPrevFreeBlock;
            }
            return mem;
        }
        pTempBlockLink = pTempBlockLink->pxNextFreeBlock;
    }
    return NULL;
}


static void stam_merge(void)
{
    
}

void *sram_mallc(unsigned int xWantedSize)
{
    char *mem;
    
    if(! HeapHead)
    {
        init_heap();
    }
    
    mem = Sramfind_heap(xWantedSize);
    if(mem)
    {
        return mem;
    }

    stam_merge();

    mem = Sramfind_heap(xWantedSize);
    if(mem)
    {
        return mem;
    }
    return NULL;
}

void sram_free( void *pv )
{
    int heapLen = 0;
    BlockLink_t *pTempHeap = find_heap(pv);
    heapLen = pTempHeap->heapLen;
    while(heapLen && pTempHeap)
    {
        pTempHeap->empty   = 1;
        pTempHeap->heapLen = 0;
        pTempHeap = pTempHeap->pxPrevFreeBlock;
        heapLen -= HeapBlockSIZE;
    }
}
