#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <iostream>
#include <Windows.h>
#include <omp.h>

//#define __COMPUTE_CUBE_SUMS_ON_THE_FLY__
#define __PRECOMPUTE_CUBE_SUMS__

#ifdef __COMPUTE_CUBE_SUMS_ON_THE_FLY__
typedef std::set<int> DataStructure;
#endif

#ifdef __PRECOMPUTE_CUBE_SUMS__
typedef std::map<int, int> DataStructure;	// Key: cube sum | Value: number of cube pairs whose sum equals Key
#endif

int main()
{
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	// Get ticks per second
	QueryPerformanceFrequency(&frequency);

	int n = 10000000;
	int number_of_cubes = pow(double(n), 1.0 / 3);

#ifdef __COMPUTE_CUBE_SUMS_ON_THE_FLY__
	DataStructure cube_values;
	for (int i = 1; i < number_of_cubes; ++i)
		cube_values.emplace(i*i*i);
#endif
#ifdef __PRECOMPUTE_CUBE_SUMS__
	DataStructure cube_sums;
	for (int i = 1; i < number_of_cubes; ++i)
	{
		for (int j = i+1; j < number_of_cubes; ++j)
		{
			int cube_sum = i*i*i + j*j*j;
			if (cube_sum <= n)
			{
				auto it = cube_sums.find(cube_sum);
				if (it == cube_sums.end())
					cube_sums[cube_sum] = 1;
				else
					++it->second;	// Increment the number of pairs whose sum equals 'cube_sum'
			}
			else
				break;
		}
	}
#endif

	// Start timer
	QueryPerformanceCounter(&t1);

#ifdef __COMPUTE_CUBE_SUMS_ON_THE_FLY__
	#pragma omp parallel for
	for (int i = 0; i < n; ++i)
	{
		int count = 0;
		int cube_found = 0;
		for (auto cube : cube_values)
		{
			int difference = i - cube;
			if (difference <= 0)
				break;
			// Check whether i can be written as a sum of 2 cubes and that the pair of cubes has not already been found 
			if (cube_values.find(difference) != cube_values.end() && difference != cube_found)
			{
				cube_found = cube;
				++count;
			}
			if (count == 2)
			{
				#pragma omp critical
				{
					std::cout << i << std::endl;
				}
				break;
			}
		}
	}
#endif

#ifdef __PRECOMPUTE_CUBE_SUMS__
	#pragma omp parallel for
	for (int i = 0; i < n; ++i)
	{
		auto it = cube_sums.find(i);
		if (it != cube_sums.end())
		{
			if (it->second >= 2)
			#pragma omp critical
			{
				std::cout << i << std::endl;
			}
		}
	}
#endif

	// Stop timer
	QueryPerformanceCounter(&t2);

	std::cout << "Elapsed time: " << (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart << " milliseconds " << std::endl;

	system("pause");
	return 0;
}