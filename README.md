Ramanujan
=========

Ramanujan n-way solutions are positive integers that are the sum of 2 cubes of positive integers in at least n ways.

Testing different implementation to compute Ramunujan 2-way solutions up to n:
- precomputing the sums of cubes: there are O(n^(2/3)) such sums which are less than or equal to n. The number of different pair of cubes whose sum equals a same value is stored in a dictionary. Using a binary tree (e.g. std::map), each lookup in the aforementioned dictionary takes O(2log(n)/3). Therefore, the worst-case complexity to find all the Ramunujan 2-way solutions up to n is O(nlog(n)). In the implementation, you can choose between std::map and std::unordered_map as the underlying dictionary structure.

- computing the sums of cubes on the fly: there are O(n^(1/3)) cubes which are less than or equal to n. For each integer i in [1;n], for each of the cubes c, we look for (i-c) in the cube list, which is stored in a binary tree (std::set). Each look up takes O(log(n)/3). As there are O(n^(1/3)) possible values for (i-c), each i is dealt with in O(n^(1/3)log(n)). Therefore, the total complexity is O(n^(4/3)log(n)). It does not seem so far from the complexity with the precomputing scheme above, but when the program is run, it does take significantly longer... Going through the elements of set may also account for the substantial decrease in performance (even if the complexity is linear) as its elements are not stored contiguously.

Additional reading
==================

http://www.durangobill.com/Ramanujan.html