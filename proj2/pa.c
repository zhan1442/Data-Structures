#include <stdio.h>
#include <stdlib.h>

void printArray(int * array, int size){

	int i;
	printf("[ ");
	for (i = 0; i < size; i++)
		printf("%d ", array[i]);
	printf("]\n");
}

int findLargestNum(int * array, int size){

	int i;
	int largestNum = -1;

	for(i = 0; i < size; i++){
		if(array[i] > largestNum)
			largestNum = array[i];
	}

	return largestNum;
}

void radixSort(int * array, int size, int largest){
	int i;
	int semiSorted[size];
	int significantDigit = 1;
	int largestNum = largest;

	while (largestNum / significantDigit > 0){
		int bucket[10] = { 0 };

		for (i = 0; i < size; i++)
			bucket[(array[i] / significantDigit) % 10]++;

		for (i = 1; i < 10; i++)
			bucket[i] += bucket[i - 1];

		for (i = size - 1; i >= 0; i--)
			semiSorted[--bucket[(array[i] / significantDigit) % 10]] = array[i];

		for (i = 0; i < size; i++)
			array[i] = semiSorted[i];

		significantDigit *= 10;
	}
}

void counting_sort(int a[], int a_len, int maxVal)
{
	int i, j, outPos = 0;
	int bucket_len = maxVal+1;
	int * bucket = (int *)calloc(bucket_len+1, sizeof(int)); /* simple bucket structure */

	/* one loop bucket processing */
	for (i = 0; i < a_len; i++)
	{
		bucket[a[i]]++; /* simple work with buckets */
	}

	for (i=0; i < bucket_len; i++)
	{
		for (j = 0; j < bucket[i]; j++)
		{
			a[outPos++] = i;
		}
	}
}

int main(){

	printf("\n\nRunning Radix Sort Example in C!\n");
	printf("----------------------------------\n");

	int size = 12;
	int list[] = {10, 2, 303, 4021, 293, 1, 0, 429, 480, 92, 2999, 14};

	printf("\nUnsorted List: ");
	printArray(&list[0], size);

	//radixSort(&list[0], size, findLargestNum(list, size));
	counting_sort(&list[0], size, findLargestNum(list, size));

	printf("\nSorted List:");
	printArray(&list[0], size);
	printf("\n");

	return 0;
}





