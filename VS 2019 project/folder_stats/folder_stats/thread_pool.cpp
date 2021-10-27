#include "thread_pool.h"

Thread_pool::Thread_pool(size_t thread_amount, const std::queue<fs::path> files_for_analysis, std::vector<long>* res)
{
	this->files_for_analysis = files_for_analysis;
	res->clear();
	res->shrink_to_fit();
	if (thread_amount <= std::thread::hardware_concurrency()) {
		this->res_vector_ptr = res;
		initialize(thread_amount);
	}
	else {
		std::cout << "Your computer doesen`t support such ammount of threads. Max amount:" << std::thread::hardware_concurrency() << std::endl;
	}
}

size_t Thread_pool::number_of_active_threads() const
{
	return threads.size();
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
			bool analyze = false;
			while (true) {
				fs::path file_address;
				{
					std::unique_lock<std::mutex> ul(m);
					cv.wait(ul, [=] {
						return stop_threads || !files_for_analysis.empty();
					});

					if (stop_threads && files_for_analysis.empty()) {
						break;
					}
					
					if (!files_for_analysis.empty()) {
						file_address = files_for_analysis.front();
						files_for_analysis.pop();
						analyze = true;

					}
				}
				if (analyze) {
					analyze_file(file_address);
					analyze = false;
				}
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
	res_vector_ptr->push_back(empty_lines.load());
	res_vector_ptr->push_back(non_empty_lines.load());
	res_vector_ptr->push_back(words.load());
	res_vector_ptr->push_back(letters.load());
}
