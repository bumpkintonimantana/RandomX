/*
Copyright (c) 2019 tevador

This file is part of RandomX.

RandomX is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RandomX is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RandomX.  If not, see<http://www.gnu.org/licenses/>.
*/

#ifndef RANDOMX_H
#define RANDOMX_H

/*

Minimal usage example:
----------------------

#include "randomx.h"
#include <stdio.h>

int main() {
  const char mySeed[] = "RandomX example seed";
  const char myInput[] = "RandomX example input";
  char hash[RANDOMX_HASH_SIZE];

  randomx_cache *myCache = randomx_alloc_cache(RANDOMX_FLAG_DEFAULT);
  randomx_init_cache(myCache, mySeed, sizeof mySeed);
  randomx_vm *myMachine = randomx_create_vm(RANDOMX_FLAG_DEFAULT);
  randomx_vm_set_cache(myMachine, myCache);

  randomx_calculate_hash(myMachine, myInput, sizeof myInput, hash);

  randomx_destroy_vm(myMachine);
  randomx_release_cache(myCache);

  for (unsigned i = 0; i < RANDOMX_HASH_SIZE; ++i)
    printf("%02x", hash[i]);

  printf("\n");

  return 0;
}

Optimized usage example:
------------------------

#include "randomx.h"
#include <stdio.h>

int main() {
  const char mySeed[] = "RandomX example seed";
  const char myInput[] = "RandomX example input";
  char hash[RANDOMX_HASH_SIZE];

  randomx_cache *myCache = randomx_alloc_cache(RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES);
  randomx_init_cache(myCache, mySeed, sizeof mySeed);

  randomx_dataset *myDataset = randomx_alloc_dataset(RANDOMX_FLAG_LARGE_PAGES);
  randomx_init_dataset(myDataset, myCache, 0, RANDOMX_DATASET_BLOCKS);
  randomx_release_cache(myCache);

  randomx_vm *myMachine = randomx_create_vm(RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES);
  randomx_vm_set_dataset(myMachine, myDataset);

  randomx_calculate_hash(myMachine, myInput, sizeof myInput, hash);

  randomx_destroy_vm(myMachine);
  randomx_release_dataset(myDataset);

  for (unsigned i = 0; i < RANDOMX_HASH_SIZE; ++i)
    printf("%02x", hash[i]);

  printf("\n");

  return 0;
}
  
*/

#include <stddef.h>

#define RANDOMX_HASH_SIZE 32
#define RANDOMX_DATASET_BLOCKS 33554432UL

typedef enum {
  RANDOMX_FLAG_DEFAULT = 0,
  RANDOMX_FLAG_FULL_MEM = 1,
  RANDOMX_FLAG_JIT = 2,
  RANDOMX_FLAG_HARD_AES = 4,
  RANDOMX_FLAG_LARGE_PAGES = 8,
} randomx_flags;

typedef struct randomx_dataset randomx_dataset;
typedef struct randomx_cache randomx_cache;
typedef struct randomx_vm randomx_vm;

#if defined(__cplusplus)
extern "C" {
#endif

randomx_cache *randomx_alloc_cache(randomx_flags flags);
void randomx_init_cache(randomx_cache *cache, const void *seed, size_t seedSize);
void randomx_release_cache(randomx_cache* cache);

randomx_dataset *randomx_alloc_dataset(randomx_flags flags);
void randomx_init_dataset(randomx_dataset *dataset, randomx_cache *cache, unsigned long startBlock, unsigned long blockCount);
void randomx_release_dataset(randomx_dataset *dataset);

randomx_vm *randomx_create_vm(randomx_flags flags);
void randomx_vm_set_cache(randomx_vm *machine, randomx_cache* cache);
void randomx_vm_set_dataset(randomx_vm *machine, randomx_dataset *dataset);
void randomx_destroy_vm(randomx_vm *machine);

void randomx_calculate_hash(randomx_vm *machine, void *input, size_t inputSize, void *output);

#if defined(__cplusplus)
}
#endif

#endif