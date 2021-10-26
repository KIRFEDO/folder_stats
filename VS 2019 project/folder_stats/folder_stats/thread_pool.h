#pragma once
#include <thread>
#include <regex>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <condition_variable>
#include <functional>
#include <filesystem>
#include <iostream>

#define MAX_THREAD_AMOUNT std::thread::hardware_concurrency()-1;
namespace fs = std::filesystem;

class Thread_pool {
	public:
		Thread_pool(size_t thread_amount, const std::queue<fs::path> files_for_analysis);
		~Thread_pool();
	private:
		std::vector<std::thread> threads;
		std::condition_variable cv;
		std::mutex m;
		bool stop_threads = false;

		void analyze_file(fs::path file_address);

		std::queue<fs::path> files_for_analysis;
		std::atomic<long> empty_lines = 0;
		std::atomic<long> non_empty_lines = 0;
		std::atomic<long> words = 0;
		std::atomic<long> letters = 0;

		void initialize(size_t thread_amount);
		void stop() noexcept;
};