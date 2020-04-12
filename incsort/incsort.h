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
					value_type pivot = *(current + (mt() % range_size));
					auto it = std::partition(
											 current,
											 stack.back(),
											 [=](const value_type& v) { return v < pivot; });
					while (it == current) {
						pivot = *(current + (mt() % range_size));
						it = std::partition(
											current,
											stack.back(),
											[=](const value_type& v) { return v < pivot; });
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



























