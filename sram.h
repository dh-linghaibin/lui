/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef __SRAM_H
#define __SRAM_H

#define debug_pt(...) //printf(__VA_ARGS__)

void *sram_mallc(unsigned int size);
void sram_free(void *pv);
void trace_heap(void);

#endif
