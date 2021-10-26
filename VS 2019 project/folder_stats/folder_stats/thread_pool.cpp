#include "thread_pool.h"

Thread_pool::Thread_pool(size_t thread_amount, const std::queue<fs::path> files_for_analysis)
{
	this->files_for_analysis = files_for_analysis;
	initialize(thread_amount);
}

Thread_pool::~Thread_pool()
{
	stop();
}

void Thread_pool::analyze_file(fs::path file_address)
{
	std::ifstream f;
	std::regex reg("\\w+");
	std::smatch m;
	f.open(file_address);
	{
		std::string buff;
		while (getline(f, buff)) {
			if (buff.size()) {
				non_empty_lines++;
				while (std::regex_search(buff, m, reg)) {
					for (const auto x : m) {
						words++;
						letters += x.str().size();
					}
					buff = m.suffix().str();
				}
			}
			else {
				empty_lines++;
			}
		}
	}
	f.close();
}

void Thread_pool::initialize(size_t thread_amount)
{
	for (auto i = 0u; i < thread_amount; i++) {
		threads.emplace_back([=]() {
			while (true) {
				fs::path file_address;
				{
					std::unique_lock<std::mutex> ul(m);
					cv.wait(ul, [=] {
						return stop_threads || files_for_analysis.empty();
					});

					if (stop_threads && files_for_analysis.empty()) {
						break;
					}

					file_address = files_for_analysis.front();
					files_for_analysis.pop();
				}
				analyze_file(file_address);
			}
		}
		);
	}
}

void Thread_pool::stop() noexcept
{
	{
		std::unique_lock<std::mutex> lock(m);
		stop_threads = true;
	}
	cv.notify_all();
	for (auto& t : threads) {
		t.join();
	}
	std::cout << "empty_lines:" << empty_lines.load() << std::endl;
	std::cout << "non_empty_lines:" << non_empty_lines.load() << std::endl;
	std::cout << "words:" << words.load() << std::endl;
	std::cout << "letters:" << letters.load() << std::endl;
}
