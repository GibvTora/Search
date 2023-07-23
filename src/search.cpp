#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <filesystem>
#include <mutex>
#include "search_lib.h"

// Define the maximum number of execution threads
const int MAX_EXECUTION_THREADS = 4;

// Mutex for synchronization between threads
std::mutex mtx;

// Function to search for the file in a directory
void searchFile(const std::string& filename, const std::filesystem::path& directory) {
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_directory()) {
                // Create new searching thread for sub-directory if the thread limit is not reached
                if (std::thread::hardware_concurrency() < MAX_EXECUTION_THREADS) {
                    std::thread([&filename, &entry]() {
                        searchFile(filename, entry.path());
                    }).detach();
                } else {
                    // If the thread limit is reached, search in the sub-directory on the current thread
                    searchFile(filename, entry.path());
                }
            } else if (entry.is_regular_file() && entry.path().filename() == filename) {
                // File found, print the full path and notify other threads to stop
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "File found: " << entry.path() << std::endl;
                std::cout << "Stopping all threads..." << std::endl;
                std::exit(0);
            }
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        // Handle the filesystem error gracefully, such as skipping the directory
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}
