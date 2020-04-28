#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


typedef struct Lock {

    int state;                                       // values of status determine the state of lock
						      // 0: Lock is free,  1: lock acquired  						
                                          
}my_struct;

int testAndSet (int* oldPtr, int new){                                                       
    int old;
    #pragma omp atomic capture              // omp atomic so that only one thread can access at one time
    {
        old = *oldPtr;			   // if previous state(oldPtr) is 0 then it will return 0 --> unlock state 					           // and set state to 1--> locked
        *oldPtr = new;                     // set state of lock to new(1) i.e. Lock is not free
    }
    return old;				// return previous state
}

void mylock_init(my_struct *lock)	// Initialize the lock by making its state 0 --> lock is free
{
    lock->state = 0;
}

void mylock_lock(my_struct *lock)	// for using lock thread have to call this function to acquire lock
{
    while(testAndSet(&lock->state,1) == 1);	// check the state of lock if it free(state=0) then lock is acquired  							//otherwise thread has to wait (while loop)
}

void mylock_unlock(my_struct *lock)   // when a thread holding the lock wants to release it then state of lock will be set to 0(free)
{
    lock->state = 0;
}

void mylock_destroy(my_struct *lock)   // Destroy the lock
{
    free(lock);				// free the memory used by lock
}

int main()
{
    long long  n;
    printf("Enter the size of array\n");
    scanf("%lld",&n);

   int t;
   printf("Enter the number of threads to use\n");
   scanf("%d",&t);

    int arr[n+1];					// create array of input size. 
    long long i=0;
    for(i=0;i<n;i++)
        arr[i] = 1;					// initialize input array with 1

    long long sum=0;

    my_struct *lock = malloc(sizeof(my_struct));       // creating a lock using the structure my_struct
    mylock_init(lock);					// initialize the lock

     omp_set_num_threads(t);  				// set number of threads to use
                                    
    double t1,t2;
    t1 = omp_get_wtime();
    #pragma omp parallel default (shared)
    {
      long long psum = 0;				// each thread will calculate the its sum and store it in psum
      #pragma omp for
      for(i=0;i<n;i++)
	psum+=arr[i];

      mylock_lock(lock);				// when a thread has to add its sum to answer it has to acquire a lock to do so
       sum+=psum;					// this is to avoid any conflict between multiple threads which want to write
      mylock_unlock(lock);				// release the lock after its job is done so that other can use the lock
   }
    t2 = omp_get_wtime();
    printf("Sum of array is %lld\n",sum);
    printf("Time taken is %f\n",(t2-t1));
    mylock_destroy(lock);				// destroy the lock when final answer is calculated

    return 0;
}
