#include <iostream>
#include <string>
#include <thread>
#include <filesystem>
#include "search_lib.h"

int main() {
    std::string filename;
    std::cout << "Enter the filename to search for: ";
    std::cin >> filename;

    std::filesystem::path rootDir("/"); // Root directory on Linux

    // Create initial searching thread for the root directory
    std::thread initialThread(searchFile, filename, rootDir);

    // Wait for the file to be found or all threads to finish
    initialThread.join();

    return 0;
}
