#pragma once

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

namespace fs = std::filesystem;

auto getFilePathsElements(const fs::path& root, const std::unordered_map<std::string, bool>& extensions_filter = {}) {
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
            }
            catch (std::out_of_range&) {}
        }
    }
    return make_pair(file_counter, files_for_analyzing);
}

void print_res(const std::vector<long int>& res) {
    if (res.size() != 4) {
        throw std::runtime_error("Result vector is damaged");
    }
    std::cout << "Number of empty lines:" << res[0] << std::endl;
    std::cout << "Number of non-empty lines:" << res[1] << std::endl;
    std::cout << "Number of words lines:" << res[2] << std::endl;
    std::cout << "Number of letters lines:" << res[3] << std::endl;
}