#pragma once
#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <functional>
#include <unordered_map>
#include <queue>
#include <utility>
#include "functions.h"
#include "thread_pool.h"

namespace fs = std::filesystem;

//getFilePathsElements

std::string getFilePathsElements_tests_path = "../../test_cases/getFilePathsElements_tests/";
std::string threadPool_tests_path = "../../test_cases/threadPool_tests/";

TEST(getFilePathsElements_test, empty_dir) {
	auto root = fs::path(getFilePathsElements_tests_path+"empty_dir");
	auto res = getFilePathsElements(root, {});
	ASSERT_TRUE(res.first == 0);
	ASSERT_TRUE(res.second.size() == 0);
}

TEST(getFilePathsElements_test, files_count_empty_filter) {
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count/1_file");
		auto res = getFilePathsElements(root, {});
		ASSERT_TRUE(res.first == 1);
		ASSERT_TRUE(res.second.size() == 0);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count/3_files");
		auto res = getFilePathsElements(root, {});
		ASSERT_TRUE(res.first == 3);
		ASSERT_TRUE(res.second.size() == 0);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count/5_files");
		auto res = getFilePathsElements(root, {});
		ASSERT_TRUE(res.first == 5);
		ASSERT_TRUE(res.second.size() == 0);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count");
		auto res = getFilePathsElements(root, {});
		ASSERT_TRUE(res.first == 9);
		ASSERT_TRUE(res.second.size() == 0);
	}
}

TEST(getFilePathsElements_test, files_count_with_filter) {
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count_filter/2_files_diff_ext");
		auto res = getFilePathsElements(root, {
			{".txt", true}
		});
		ASSERT_TRUE(res.first == 2);
		ASSERT_TRUE(res.second.size() == 1);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count_filter/2_files_diff_ext");
		auto res = getFilePathsElements(root, {
			{".log", true}
		});
		ASSERT_TRUE(res.first == 2);
		ASSERT_TRUE(res.second.size() == 1);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count_filter/2_files_diff_ext");
		auto res = getFilePathsElements(root, {
			{".log", true},
			{".txt", true}
		});
		ASSERT_TRUE(res.first == 2);
		ASSERT_TRUE(res.second.size() == 2);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count_filter/6_files_diff_ext");
		auto res = getFilePathsElements(root, {
			{".txt", true}
			});
		ASSERT_TRUE(res.first == 6);
		ASSERT_TRUE(res.second.size() == 5);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count_filter/6_files_diff_ext");
		auto res = getFilePathsElements(root, {
			{".log", true}
			});
		ASSERT_TRUE(res.first == 6);
		ASSERT_TRUE(res.second.size() == 1);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count_filter/6_files_diff_ext");
		auto res = getFilePathsElements(root, {
			{".log", true},
			{".txt", true}
			});
		ASSERT_TRUE(res.first == 6);
		ASSERT_TRUE(res.second.size() == 6);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count_filter");
		auto res = getFilePathsElements(root, {
			{".txt", true}
		});
		ASSERT_TRUE(res.first == 8);
		ASSERT_TRUE(res.second.size() == 6);
	}
	{
		auto root = fs::path(getFilePathsElements_tests_path+"files_count_filter");
		auto res = getFilePathsElements(root, {
			{".txt", true},
			{".log", true}
		});
		ASSERT_TRUE(res.first == 8);
		ASSERT_TRUE(res.second.size() == 8);
	}
}

//threadPool

TEST(threadPool_test, number_of_threads) {
	std::vector<long> v;
	{
		Thread_pool tp(5, {}, &v);
		ASSERT_TRUE(tp.number_of_active_threads() == 5);
	}
	{
		Thread_pool tp(3, {}, &v);
		ASSERT_TRUE(tp.number_of_active_threads() == 3);
	}
	{
		Thread_pool tp(1, {}, &v);
		ASSERT_TRUE(tp.number_of_active_threads() == 1);
	}
}

TEST(threadPool_test, empty_dir) {
	std::vector<long> v;
	auto root = fs::path(getFilePathsElements_tests_path+"files_count/5_files");
	auto res = getFilePathsElements(root, {});
	{
		Thread_pool t(5, res.second, &v);
	}
	ASSERT_TRUE(v[0] == 0);
	ASSERT_TRUE(v[1] == 0);
	ASSERT_TRUE(v[2] == 0);
	ASSERT_TRUE(v[3] == 0);
}

TEST(threadPool_test, analyze_test) {
	std::vector<long> v;
	{
		auto root = fs::path(threadPool_tests_path + "file_group_1");
		auto res = getFilePathsElements(root, {
			{".txt", true}
		});
		{
			Thread_pool tp(2, res.second, &v);
		}
		ASSERT_TRUE(v[0] == 6);
		ASSERT_TRUE(v[1] == 6);
		ASSERT_TRUE(v[2] == 6);
		ASSERT_TRUE(v[3] == 6);
	}
	{
		auto root = fs::path(threadPool_tests_path + "file_group_2");
		auto res = getFilePathsElements(root, {
			{".txt", true}
			});
		{
			Thread_pool tp(1, res.second, &v);
		}
		ASSERT_TRUE(v[0] == 0);
		ASSERT_TRUE(v[1] == 1);
		ASSERT_TRUE(v[2] == 5);
		ASSERT_TRUE(v[3] == 10);
	}
	{
		auto root = fs::path(threadPool_tests_path + "file_group_3");
		auto res = getFilePathsElements(root, {
			{".txt", true}
			});
		{
			Thread_pool tp(1, res.second, &v);
		}
		ASSERT_TRUE(v[0] == 6);
		ASSERT_TRUE(v[1] == 3);
		ASSERT_TRUE(v[2] == 15);
		ASSERT_TRUE(v[3] == 30);
	}
}