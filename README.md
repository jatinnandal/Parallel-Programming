# 1.Subset Sum Problem:

Write a task based OpenMP program in C that implements the dynamic program to solve the subset sum problem with memoization using recursion. Assume inputs are non-negative integers only and n=number of inputs and S=total desired sum are provided as input. Assume that the function is invoked from a parallel region (i.e., you should not have the #pragma omp parallel construct inside your function).

# 2.Implementation of Lock using omp atomic:

Define a suitable data structure my_struct and write routines mylock_init(my_struct), mylock_lock(my_struct), mylock_unlock(my_struct) and
mylock_destroy(my_struct) to implement locks. You are only allowed to use #pragma omp atomic to implement the lock.


To run these code use gcc - fopenmp.




