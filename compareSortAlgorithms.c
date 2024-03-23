#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

//helper function for heapsort
//makes max heap
//inspired by code from geeksforgeeks.org
void heapify (int* pData, int n, int i)
{
	//declare working variables
	int max = i; //index of max value
	int l = 2*i; //index of left child
	int r = 2*i + 1; //index of right child
	int temp; //used for swaping values
	
	//check if children out of bounds
	if (l >= n || r >= n)
		return;

	//find max value
	if (pData[l] > pData[max])
		max = l;
	
	if (pData[r] > pData[max])
		max = r;
	
	//execute heapify
	if (max != i) //checks if any change necessary
	{
		//sort
		temp = pData [max];
		pData[max] = pData[i];
		pData[i] = pData[max];

		//make recursive call
		heapify(pData, n, max);
	}
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	int i, temp;
	int first_leaf = n/2; //index of firt leaf
	// make maxheap

	//make max heap
	for (i = first_leaf -1; i > -1; i--)
		heapify (arr, n, i);
	
	for (i = n-1; i > 0; i--)
	{
		//swap each value
		temp = arr[i];
		arr[i] = arr[0];
		arr[0] = temp;

		//repair heap property
		heapify(arr, i, 0);
	}	
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	//not reusing lab code
	
	/*Steps:
		1. implement terminating condition
		2. recursion
			call mergesort on left part
			call mergsort on right part
		3.merge the parts
			get temp arrays
			LOOP:	if value in left bigger than in right
						put left first
					else put right first
			add any remaining data from left,
			then add from right
	*/

	//Check terminating condition
	if (l>= r)
		return;
	//else proceed

	//Declare temp variables and arrays
	int i, j, k; //counters
	int mid = (l + r) / 2;
	int n1 = mid - l +1;
	int n2 = r - mid;
	int * right = Alloc(( r-l) / 2); //right sub array
	int * left = Alloc(( r-l) / 2); //left sub array

	//Make recursive calls
	mergeSort(pData, l, mid);
	mergeSort(pData, mid+1, r);

	//Populate temp arrays for merge
	for (i = 0; i < n1; i++)
		left [i] = pData[i + l];
	for (j = 0; j < n2; j++)
		right [j] = pData[j + mid + 1];
	
	//initialise counters
	i = 0; //left sub-array counter
	j = 0; //right sub-array counter
	k = l; //primary array (pData) counter

	//merge 
	while (i < n1 && j < n2)
	{
		if (left [i] < right [j]) //left array value smaller
		{
			pData[k] = left[i];
			i++;
		}
		else //right array value smaller
		{
			pData[k] = right [j];
			j++;
		}

		k++;
	}

	//handle left over values
	for (i; i < n1; i++)
	{
		pData [k] = left [i];
		k++;
	}

	for (j; j < n2; j++)
	{
		pData [k] =  right [j];
		k++;
	}
	//free memory
	DeAlloc(right);
	DeAlloc(left);
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	/*Steps
		1.  traverse from 0 to n
		2.	initiate second loop from current-1 to 0
		3. 	store value at current
		4. 	in inner loop, slide values left 
			until stored value is in correct place
		5. replace stored value
	*/
	//variable declaration
	int i, j, stored;

	for (i = 1; i < n; i++) //primary traversal loop
	{
		stored = pData[i]; //stores current value

		for (j = i-1; j >-1; j--)
		{
			if (pData[j] > stored) //current item larger than stored
				pData[j+1] = pData[j]; //shift
			
			else
				break; //in correct place
		}

		pData[j+1] = stored; //replaces stored value;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	/*Steps
		1. traverse array
		2. traverse again until current point
			if data at second larger
			than data a first, swap 
	*/

	int temp;
	for (int i = 0; i < n - 1; i++) //primary traversal loop
	{
		for (int j = 0; j < n - i - 1; j++) //looks for smaller proceeding value
		{
			if (pData [j] > pData [j+1]) //swap everytime smaller value found
			{
				temp = pData [j];
				pData [j] = pData[j+1];
				pData[j+1] = temp;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	/*Steps:
		1. traverse array until 2nd to last
		2. set min index current
		3. traverse rest of array until end
			if value smaller, set min
		4. if min not current, swap values
	*/

	//Variable declaration
	int temp, min;

	for (int i = 0; i < n-1; i ++) //primary traversal loop
	{	
		min = i; //sets smallest to current
		for (int j = i; j < n; j++) //finds smallest proceeding value
		{
			if (pData[j] < pData [min]) //compares values at indecies
				min = j;
		}

		if (min == i) //no need to swap
			continue;

		//swaps only smallest value
		temp = pData [min];
		pData [min] = pData [i];
		pData [i] = temp;
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	int n, i, * data;
	
	if (inFile)
	{
		fscanf(inFile,"%d",&dataSz);
		*ppData = (int*) Alloc(sizeof(int) * dataSz);
		if (*ppData == NULL)
			exit;

		// Implement parse data block
		for (i = 0; i < dataSz-1; i++)
		{
			fscanf(inFile, "%d", &n);
			(*ppData)[i] = n;
		}
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
