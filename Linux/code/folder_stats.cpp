#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <map>
#include <chrono>
#include <utility>
#include <atomic>
#include <fstream>
#include "functions.h"
#include "thread_pool.h"
#include "unit_tests.h"

namespace fs = std::filesystem;

int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    auto folder_for_analysis = fs::path("../tests_cases"); 
    if (fs::exists(folder_for_analysis)) {
        std::vector<long> v;
        std::unordered_map<std::string, bool> filter = {
            {".txt", true},
        };
        auto folder_stat = getFilePathsElements(folder_for_analysis, filter);
        for (auto i = 1u; i < std::thread::hardware_concurrency() - 1; i++) {
            std::cout << "NUMBER OF FILES:" << folder_stat.first << std::endl;
            std::cout << "[=================================================================]" << std::endl;
            auto start = std::chrono::high_resolution_clock::now();
            {
                Thread_pool tp(i, folder_stat.second, &v);
                std::cout << "Number of threads:" << tp.number_of_active_threads() << std::endl;
            }
            auto end = std::chrono::high_resolution_clock::now();
            print_res(v);
            std::chrono::duration<float> duration = end - start;
            std::cout << "Duration:" << duration.count() << 's' << std::endl;
            std::cout << "[=================================================================]" << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }
    else {
        std::cout << std::endl << "Error:No such folder" << std::endl;
    }
    return 0;
}