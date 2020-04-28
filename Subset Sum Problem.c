#include<stdio.h>
#include<stdbool.h>
#include<omp.h>

int isSubsetSum(int arr[], int n, int sum, int visited[n][sum]) 
{ 
   int x,y;
   if (sum == 0){			// Base Case 
     return 1;	
   } 
	
	if(sum<0 || n<=0)		// Base Case
		return 0;
    
   if(visited[n][sum]!=-1)						// if already calculated (memoization)
		return visited[n][sum];

	#pragma omp task shared(x)
	x = isSubsetSum(arr, n-1, sum,visited);			//excluding the current element --> Task1

	#pragma omp task shared(y)
	y = isSubsetSum(arr, n-1, sum-arr[n-1],visited);  // including the current element --> Task2

	#pragma omp taskwait						//wait for both the task to complete
	visited[n][sum] = x||y;						// store value for memoization

    return visited[n][sum];
} 


int main(){
	int n,s;
	printf("Enter the value of n \n");			// number of elements
	scanf("%d",&n);

	printf("Enter the value of s \n");			// required sum
	scanf("%d",&s);
	
	int arr[n+1];
	
	printf("Enter the values of array\n");
	for(int i=0;i<n;i++){
		scanf("%d",&arr[i]);
	}

	int visited[n+1][s+1];					// cache for memoization
	
	for(int i=0;i<=n;i++){
		for(int j=0;j<=s;j++){
			visited[i][j] = -1;				// Initialize visited array
		}
	}

	int ans;
	
	#pragma omp parallel
	#pragma omp single						// only one thread should call function
	ans = isSubsetSum(arr, n, s,visited);

	if(ans)
		printf("Found a subset with given sum");
	else
		printf("No subset with given sum");

}
