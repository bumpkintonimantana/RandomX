#include <randomx.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>

int main() {
    // Initialize RandomX
    randomx_flags flags = randomx_get_flags();
    randomx_cache *cache = randomx_alloc_cache(flags);
    randomx_init_cache(cache, "RandomX key", 10);
    randomx_vm *vm = randomx_create_vm(flags, cache, NULL);

    // Set the target number of iterations per second and run duration as variables
    int targetRate = 10000; // Change this number to adjust the speed
    int runDuration = 1000; // Change this number to adjust how long the program runs (in seconds)

    // Calculate target duration based on the rate
    const std::chrono::milliseconds targetDuration(1000 / targetRate);

    // Run for 'runDuration' seconds
    auto start = std::chrono::steady_clock::now();
    int count = 0;
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < runDuration) {
        auto iterStart = std::chrono::steady_clock::now();

        // Modify input on each iteration
        std::stringstream ss;
        ss << "RandomX input " << count;
        std::string inputStr = ss.str();
        char* input = const_cast<char*>(inputStr.c_str());

        // RandomX operation, like hashing
        char hash[RANDOMX_HASH_SIZE];
        randomx_calculate_hash(vm, input, inputStr.size(), &hash);

        // Log the hash
        std::cout << "Hash " << std::setw(5) << count << ": ";
        for (unsigned char c : hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
        }
        std::cout << std::endl;

        ++count;  // Increment count

        // Calculate how long the iteration took and sleep accordingly to maintain the rate
        auto iterEnd = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(iterEnd - iterStart);
        if (duration < targetDuration) {
            std::this_thread::sleep_for(targetDuration - duration);
        }
    }

    // Clean up
    std::cout << "Cleaning up and exiting..." << std::endl;
    randomx_destroy_vm(vm);
    randomx_release_cache(cache);

    return 0;
}
