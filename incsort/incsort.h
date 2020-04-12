//
//  incsort.h
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

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>


// Pre-sorting
template<typename I>
class pre_sorter {
public:
	pre_sorter(I i1, I i2) : first(i1), last(i2) {
		std::sort(first, last);
	}
	
	I begin() { return first; }
	I end() { return last; }
	
private:
	I first;
	I last;
};

// Partial sorting
template<typename I>
class partial_sorter {
public:
	typedef typename std::iterator_traits<I>::value_type value_type;
	
	partial_sorter(I i1, I i2) : first(i1), last(i2) {}
	
	class iterator {
	public:
		iterator(I first, I last) : current(first), sort_end(first), end(last) {}
		
		iterator& operator++() {
			ensure_sorted_at_current();
			++current;
			return *this;
		}
		
		value_type& operator*() {
			ensure_sorted_at_current();
			return *current;
		}
		
		bool operator==(const iterator &iter) { return current == iter.current; }
		bool operator!=(const iterator &iter) { return current != iter.current; }
		
	private:
		void ensure_sorted_at_current() {
			if (current == sort_end) {
				sort_end = (sort_size < end - sort_end) ? sort_end + sort_size : end;
				std::partial_sort(current, sort_end, end);
				sort_size *= 2;
			}
		}
		I current;
		I sort_end;
		I end;
		int sort_size = 100;
	};
	
	iterator begin(void) { return iterator(first, last); }
	iterator end(void) { return iterator(last, last); }
	
private:
	I first;
	I last;
};

// Incremental Quicksort
template<typename I>
class simple_quick_sorter {
public:
	typedef typename std::iterator_traits<I>::value_type value_type;
	
	simple_quick_sorter(I i1, I i2) : first(i1), last(i2) {}
	
	class iterator {
	public:
		iterator(I first, I last) : current(first), sort_end(first) { stack.push_back(last); }
		
		iterator& operator++() {
			ensure_sorted_at_current();
			++current;
			return *this;
		}
		
		value_type& operator*() {
			ensure_sorted_at_current();
			return *current;
		}
		
		bool operator==(const iterator &iter) { return current == iter.current; }
		bool operator!=(const iterator &iter) { return current != iter.current; }
		
	private:
		void ensure_sorted_at_current() {
			if (current == sort_end) {
				while (stack.back() - sort_end > sort_limit) {
					auto range_size = stack.back() - current;
					// BN: do some sanity checks on the pivot to avoid infinite loops
					//value_type pivot = *(current + (mt() % range_size));
                                        auto pivot_addr = current + (mt() % range_size) + 1;
                                        if (pivot_addr >= sort_end or pivot_addr == current) break;
                                        value_type pivot = *pivot_addr;

                                        auto it = std::partition(
											 current,
											 stack.back(),
											 [=](const value_type& v) { return v < pivot; });
					int64_t counter = 0;	// BCH: THIS IS DEBUG CODE, USED TO DETECT A HANG IN THE LOOP THAT FOLLOWS
					while (it == current) {
						pivot = *(current + (mt() % range_size));
						it = std::partition(
											current,
											stack.back(),
											[=](const value_type& v) { return v < pivot; });
						// BCH : HANG DETECTION.  You can confirm that the hang is real by commenting this out, of course.
						// The hang appears to occur when the whole range of values that the pivot could be drawn from is
						// identical; the partition then does nothing, and returns current, and so it == current, so we go
						// around again, choose another pivot, etc.  So the way in which the original algorithm's
						// recursion was unwound here seems to have a small bug that causes an infinite loop in that case.
						// Or maybe the bug was present in the original algorithm, too, but that seems unlikely.
						// Note that because of the way sort_limit is used to fall back to std::sort(), the number of
						// duplicate pivot values must be larger than sort_limit to hit this hang.  So you can make the
						// hang reproduce with a smaller test vector by setting sort_limit to be smaller; if you set it
						// to 5, you can change LENGTH in FAIL_TEST() to 6, and reproduce the bug with the test vector
						// [0, 0, 0, 0, 0, 0], but I have not done that here because sort_limit == 100 is the original
						// value used by Lars Hagen.  But I don't think there's anything magic about it, it's just
						// chosen heuristically to optimize performance.
						if (++counter == 10000000)
						{
							std::cout << "HANG -- EXITING" << std::endl;
							exit(EXIT_FAILURE);
						}
					}
					stack.push_back(it);
				}
				
				std::sort(sort_end, stack.back());
				sort_end = stack.back();
				stack.pop_back();
			}
		}
		I current;
		I sort_end;
		std::vector<I> stack;
		static const int sort_limit = 100;
		std::mt19937 mt{ std::random_device{}() };
	};
	
	iterator begin(void) { return iterator(first, last); }
	iterator end(void) { return iterator(last, last); }
	
private:
	I first;
	I last;
};



























