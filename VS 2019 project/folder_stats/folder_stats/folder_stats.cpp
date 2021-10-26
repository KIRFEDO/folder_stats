#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <map>
#include <chrono>
#include <utility>
#include <atomic>
#include <fstream>
#include <gtest/gtest.h>
#include "thread_pool.h"

namespace fs = std::filesystem;

auto getFilePathsElements(const fs::path& root, const std::unordered_map<std::string, bool>& extensions_filter = {}, const bool& extended_folder_stat = false){
    long long file_counter = 0;
    std::queue<fs::path> files_for_analyzing;
    for (auto& p : fs::recursive_directory_iterator(root)) {
        if (fs::is_regular_file(p)) {
            file_counter++;
            try {
                if (
                    extensions_filter.at(p.path().extension().string())
                    ) {
                    files_for_analyzing.push(p);
                }
            }catch(std::out_of_range& e){}
        }
    }
    return make_pair(file_counter, files_for_analyzing);
}

int main(int argc, char **argv) {
    auto root = fs::path("/Users/User/Desktop/BCF software/test_cases");
    std::unordered_map<std::string, bool> filter = {
        {".txt", true},
        {".cpp", true},
        {".exe", true}
    };
    auto folder_stat = getFilePathsElements(root, filter);
    Thread_pool tp(3, folder_stat.second);
    return 0;
}