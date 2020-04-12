//
//  main.cpp
//  incsort
//
//  Created by Ben Haller on 4/11/20.
//  Copyright (c) 2020 Philipp Messer.  All rights reserved.
//	A product of the Messer Lab, http://messerlab.org/slim/
//

//	This file is part of incsort.
//
//	incsort is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
//	incsort is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License along with incsort.  If not, see <http://www.gnu.org/licenses/>.

#include "incsort.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std::chrono;


typedef std::vector<int64_t> vector_type;
typedef vector_type::iterator iterator_type;


static void print_vec(std::string prefix, vector_type &v, std::string postfix = "\n")
{
	bool first = true;
	
	std::cout << prefix;
	for (const auto &e : v)
	{
		if (!first)
			std::cout << ", ";
		std::cout << e;
		first = false;
	}
	std::cout << postfix;
}

static void print_test(int64_t len)
{
	int64_t half = len >> 1;
	vector_type v;
	
	std::cout << "Basic integrity test, sorting [-" << half << ", " << (half - 1) << "]:" << std::endl << std::endl;
	
	// initial contents
	for (int64_t i = -half; i < half; ++i) v.push_back(i);
	print_vec("initial: ", v);
	
	// pre-sorted
	std::random_shuffle(v.begin(), v.end());
	print_vec("shuffled: ", v);
	
	{
		pre_sorter<iterator_type> sorter(v.begin(), v.end());
		vector_type w;
		
		for (const auto &e : sorter) w.push_back(e);
		print_vec("sorted: ", w);
	}
	
	// partial-sorted
	std::random_shuffle(v.begin(), v.end());
	print_vec("shuffled: ", v);
	
	{
		partial_sorter<iterator_type> sorter(v.begin(), v.end());
		vector_type w;
		
		for (const auto &e : sorter) w.push_back(e);
		print_vec("sorted: ", w);
	}
	
	// incrementally quick-sorted
	std::random_shuffle(v.begin(), v.end());
	print_vec("shuffled: ", v);
	
	{
		simple_quick_sorter<iterator_type> sorter(v.begin(), v.end());
		vector_type w;
		
		for (const auto &e : sorter) w.push_back(e);
		print_vec("sorted: ", w);
	}
	
	std::cout << std::endl << std::string(70, '=') << std::endl << std::endl;
}

static int64_t time_test(vector_type &initial, vector_type &shuffled, int64_t fetch_count, int algorithm)
{
	vector_type v = shuffled;
	vector_type w;
	
	w.reserve(v.size());
	
	// start clock
	auto start = high_resolution_clock::now();
	
	if (algorithm == 0)
	{
		// pre-sorted
		pre_sorter<iterator_type> sorter(v.begin(), v.end());
		auto sorted_iter = sorter.begin();
		
		for (int64_t i = 0; i < fetch_count; ++i)
			w.push_back(*(sorted_iter++));
	}
	else if (algorithm == 1)
	{
		// partial-sorted
		partial_sorter<iterator_type> sorter(v.begin(), v.end());
		auto sorted_iter = sorter.begin();
		
		for (int64_t i = 0; i < fetch_count; ++i)
		{
			w.push_back(*sorted_iter);
			++sorted_iter;
		}
	}
	else if (algorithm == 2)
	{
		// incrementally quick-sorted
		simple_quick_sorter<iterator_type> sorter(v.begin(), v.end());
		auto sorted_iter = sorter.begin();
		
		for (int64_t i = 0; i < fetch_count; ++i)
		{
			w.push_back(*sorted_iter);
			++sorted_iter;
		}
	}
	
	// stop clock
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	
	// test for correctness
	if ((w.size() != fetch_count) || !std::equal(initial.begin(), initial.begin() + fetch_count, w.begin()))
	{
		std::cout << "NOT EQUAL!" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	return duration.count();
}

static void run_time_tests(const int num_iters, const int max_power_2)
{
	std::cout << "Timing test, " << num_iters << " iterations up to 2^" << max_power_2 << " elements:" << std::endl << std::endl;
	
	int64_t length = powl(2, max_power_2);
	int64_t half = length >> 1;
	vector_type initial;
	for (int64_t i = -half; i < half; ++i)
		initial.push_back(i);
	
	std::vector<int64_t> fetch_counts, pre_times, partial_times, iqs_times;
	
	for (int64_t fetch_count = 1; fetch_count <= length; fetch_count *= 2)
	{
		fetch_counts.push_back(fetch_count);
		pre_times.push_back(0);
		partial_times.push_back(0);
		iqs_times.push_back(0);
	}
	
	// run num_iters iterations of the tests, to decrease noise
	for (int iteration = 1; iteration <= num_iters; ++iteration)
	{
		std::cout << "Iter " << iteration << " : ";
		
		// Make the shuffled vector for this iteration
		vector_type shuffled = initial;
		std::random_shuffle(shuffled.begin(), shuffled.end());
		
		for (int64_t fetch_iter = 0; fetch_iter < fetch_counts.size(); ++fetch_iter)
		{
			int64_t fetch_count = fetch_counts[fetch_iter];
			
			std::cout << fetch_count << "..";
			pre_times[fetch_iter] += time_test(initial, shuffled, fetch_count, 0);
			partial_times[fetch_iter] += time_test(initial, shuffled, fetch_count, 1);
			iqs_times[fetch_iter] += time_test(initial, shuffled, fetch_count, 2);
		}
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
	std::cout << "iter_count <- c(" << num_iters << ")\n";
	print_vec("fetch_counts <- c(", fetch_counts, ")\n");
	print_vec("pre_times <- c(", pre_times, ")\n");
	print_vec("partial_times <- c(", partial_times, ")\n");
	print_vec("iqs_times <- c(", iqs_times, ")\n");
	
	std::cout << std::endl << std::string(70, '=') << std::endl << std::endl;
}

int main(int argc, const char * argv[])
{
	// basic test
	print_test(40);
	
	// run a timing test and print R-compatible results
	run_time_tests(25, 23);
	
	return 0;
}


































