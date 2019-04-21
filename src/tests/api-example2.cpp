#include "../randomx.h"
#include <iostream>
#include <iomanip>
#include <thread>

int main() {
	const char mySeed[] = "RandomX example seed";
	const char myInput[] = "RandomX example input";
	char hash[RANDOMX_HASH_SIZE];

	randomx_cache *myCache = randomx_alloc_cache((randomx_flags)(RANDOMX_FLAG_JIT | RANDOMX_FLAG_LARGE_PAGES));
	if (myCache == nullptr) {
		std::cout << "Cache allocation failed" << std::endl;
		return 1;
	}
	randomx_init_cache(myCache, mySeed, sizeof mySeed);

	randomx_dataset *myDataset = randomx_alloc_dataset(RANDOMX_FLAG_LARGE_PAGES);
	if (myDataset == nullptr) {
		std::cout << "Dataset allocation failed" << std::endl;
		return 1;
	}
	std::thread t1(&randomx_init_dataset, myDataset, myCache, 0, RANDOMX_DATASET_BLOCKS / 2);
	std::thread t2(&randomx_init_dataset, myDataset, myCache, RANDOMX_DATASET_BLOCKS / 2, RANDOMX_DATASET_BLOCKS / 2);
	t1.join();
	t2.join();
	randomx_release_cache(myCache);

	randomx_vm *myMachine = randomx_create_vm((randomx_flags)(RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_JIT | RANDOMX_FLAG_HARD_AES | RANDOMX_FLAG_LARGE_PAGES), nullptr, myDataset);

	randomx_calculate_hash(myMachine, &myInput, sizeof myInput, hash);

	randomx_destroy_vm(myMachine);
	randomx_release_dataset(myDataset);

	for (unsigned i = 0; i < RANDOMX_HASH_SIZE; ++i)
		std::cout << std::hex << ((int)hash[i] & 0xff);

	std::cout << std::endl;

	return 0;
}