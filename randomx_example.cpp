#include <randomx.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>

int main() {
    // Initialize RandomX
    randomx_flags flags = randomx_get_flags();
    randomx_cache *cache = randomx_alloc_cache(flags);
    randomx_init_cache(cache, "RandomX key", 10);
    randomx_vm *vm = randomx_create_vm(flags, cache, NULL);

    // Run for 100 seconds
    auto start = std::chrono::steady_clock::now();
    int count = 0;
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < 100) {
        // Modify input on each iteration
        std::stringstream ss;
        ss << "RandomX input " << count;
        std::string inputStr = ss.str();
        char* input = const_cast<char*>(inputStr.c_str());

        // RandomX operation, like hashing
        char hash[RANDOMX_HASH_SIZE];
        randomx_calculate_hash(vm, input, inputStr.size(), &hash);

        // Log the hash
        if (++count % 10 == 0) {
            std::cout << "Hash " << std::setw(5) << count << ": ";
            for (unsigned char c : hash) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
            }
            std::cout << std::endl;
        }
    }

    // Clean up
    std::cout << "Cleaning up and exiting..." << std::endl;
    randomx_destroy_vm(vm);
    randomx_release_cache(cache);

    return 0;
}
