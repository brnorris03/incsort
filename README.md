# incsort

This project demonstrates an [incremental sort algorithm](<https://en.wikipedia.org/wiki/Partial_sorting#Incremental_sorting>) written in C++ that allows sorted results to be obtained from an iterator without having to sort the entire vector up front.  This allows sorting to be done while showing progress in a progress panel, for example.

The code for incsort is based upon a [blog post](http://larshagencpp.github.io/blog/2016/04/23/fast-incremental-sort) by Lars Hagen from 2016.  The code in Hagen (2016) is useful but incomplete, and he apparently never posted the full code online (and didn't respond to email).  I have therefore reconstructed his work here.  The license his partial implementation was posted under is unclear, but since he has posted [other blog-related materials](https://github.com/larshagencpp/cppwhiletrue) under the MIT license, I have presumed that that was his intention.  Note that incsort does not implement the "skewed incremental sort" algorithm of Hagen (2016), because I didn't need it for my purposes.

That blog post, in turn, was based upon the incremental quicksort, or IQS, algorithm presented by [Paredes & Navarro (2006)](https://doi.org/10.1137/1.9781611972863.16).  Note that [Regla & Paredes (2015)](https://doi.org/10.1109/SCCC.2015.7416566) gives an updated version of the IQS algorithm that is worst-case optimal; again, that algorithm is not provided in incsort.  (Feel free to do a pull request if you implement those additional algorithms.)

Included in this repository are:

- incsort/incsort.h : the three sorting methods implemented, as C++ template classes
- incsort/main.cpp : main(), with timing test code to compare the methods
- incsort.xcodeproj : an Xcode project, for those who want it, but the source is pure C++
- sort_times.R : an R script that plots timing results from Incremental_sorting
- sort_times.pdf : the resulting plot from timing tests run on my Mac mini
- LICENSE : the GNU GPL v. 3 license under which incsort is distributed (see below)
- README.md : this README markdown file

I hope this project proves useful.

<em>Benjamin C. Haller<BR>
Messer Lab, Cornell University<BR>
11 April 2020</em>


## License

Copyright (c) 2020 Philipp Messer.  All rights reserved.

incsort is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

incsort is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with incsort.  If not, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).
