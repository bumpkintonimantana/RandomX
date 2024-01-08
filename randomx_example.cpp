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

    // Timing and hash rate calculation
    auto startTime = std::chrono::steady_clock::now();
    int count = 0;
    int hashCount = 0;
    constexpr int hashRateCalcInterval = 10; // Calculate hash rate every 10 seconds
    constexpr int totalDuration = 100; // Total duration for the script to run in seconds

    while (true) {
        // Modify input on each iteration
        std::stringstream ss;
        ss << "RandomX input " << count;
        std::string inputStr = ss.str();
        char* input = const_cast<char*>(inputStr.c_str());

        // RandomX operation, like hashing
        char hash[RANDOMX_HASH_SIZE];
        randomx_calculate_hash(vm, input, inputStr.size(), &hash);
        hashCount++;
        count++; // Increment the count for each hash computed

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        auto elapsedSinceLastRate = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        // Display incremental progress
        if (elapsed <= totalDuration && elapsedSinceLastRate >= 1) {
            std::cout << elapsed << "/" << totalDuration << std::endl;
            startTime = std::chrono::steady_clock::now(); // Reset start time for incremental display
        }

        // Calculate and display hash rate every 10 seconds
        if (elapsedSinceLastRate >= hashRateCalcInterval) {
            double hashRate = hashCount / hashRateCalcInterval; // hashes per second
            std::cout << "Hash " << count << ": Current hash rate: " << hashRate << " H/s" << std::endl;

            // Reset counters for hash rate calculation
            hashCount = 0;
        }

        // Break the loop after the total duration
        if (elapsed >= totalDuration) {
            break;
        }
    }

    // Clean up
    randomx_destroy_vm(vm);
    randomx_release_cache(cache);

    return 0;
}
