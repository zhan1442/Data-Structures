#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void counting_sort(int a[], int a_len, int maxVal, int min);
void printArray(int * array, int size);
int prep( int * array, int size,  int * largest, int *smallest);
void radix_sort( int a[], int a_len, int largest, int min);

void printArray(int * array, int size){

	int i;
	printf("[ ");
	for (i = 0; i < size; i++)
		printf("%d ", array[i]);
	printf("]\n");
}

//return m/n, input &largest &smallest
int prep(int * array, int size, int * largest, int *smallest){

	int i;
	*largest = array[0];
	*smallest = array[0];

	for(i = 0; i < size; i++){
		if(array[i] > *largest) *largest = array[i];
		else if(array[i] < *smallest) *smallest = array[i];
	}
	printf("range = %d size = %d m/n = %f",*largest-*smallest,size,(*largest-*smallest)/(double) size);	
	return (*largest-*smallest)/(double) size;
}
/*
   void radixSort(int * array, int size, int largest){
   int i;
   int *semiSorted = malloc(size*sizeof(int));
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
   }*/

void radix_sort( int a[], int a_len, int largest, int min)
{
	int i;
	int *b = malloc(a_len*sizeof( int));
	int expn = 1;

	for (i = 0; (largest-min)/expn>0; ++i)
	{
		int j;
		int bucket[10] = {0}; 

		for (j = 0; j != a_len; ++j)
			bucket[ (a[j]-min) / expn % 10 ]++;

		for (j = 1; j != 10; ++j)
			bucket[j] += bucket[j - 1];

		for (j = a_len - 1; j >= 0; --j)
			b[--bucket[(a[j]-min) / expn % 10]] = a[j]-min;

		for (j = 0; j != a_len; ++j)
			a[j] = b[j]+min;

		expn *= 10;
	}
	free(b);
}


void counting_sort(int a[], int a_len, int maxVal, int min)
{
	int i, j, outPos = 0;
	int bucket_len = maxVal-min+1;
	int * bucket = (int *)calloc(bucket_len+1, sizeof(int)); /* simple bucket structure */


	/* one loop bucket processing */
	for (i = 0; i < a_len; i++)
	{
		bucket[a[i]-min]++; /* simple work with buckets */
	}

	for (i=0; i < bucket_len; i++)
	{
		for (j = 0; j < bucket[i]; j++)
		{
			a[outPos++] = i + min;
		}
	}
	free(bucket);
}

int main(int argc,char * * argv){

	//printf("\n\nRunning Radix Sort Example in C!\n");
	printf("----------------------------------\n");

	//int size = 23;
	//int list[] = {-10, -2, 303, 4021,-10, 293, 1, -10, 429, 480, -92, 2999, 14,14,203942,-21938,-21398,23894,348,23423,-934,3485,394};
	int size = 0; 
	size = atoi(argv[1]);
	//printf("size = %d\n",size);
	int * list = malloc(size*sizeof(int));
	int i;
	//double s;
	//for (s=1000;s<=100000;s+=100){
	for (i = 0;i < size;i++) if(i<size-10) list[i] = (int) (rand()%10-5)*(rand()%((i+1)*10))/2; else list[i]=(int) (rand()%10-5)*rand()/5;
	//for (i = 0;i < size;i++) list[i]=rand();
	//printf("list[0] = %d\n",list[0]);
	int largest = 0;
	int smallest =0;

//radixSort(&list[0], size, largest);
	printf("\nUnsorted List: ");
	printArray(&list[0], size);

	clock_t st;
	clock_t ed;
	st = clock();
	int mn = prep(list, size, &largest, &smallest);
	if(mn > 60) 
	{radix_sort(&list[0], size, largest,smallest);printf("\nRadix Sort\n");}
	else {counting_sort(&list[0], size, largest, smallest);printf("\ncounting Sort\n");}
	ed = clock();
	printf("Sorted List:");
	printArray(&list[0], size);
	printf("time = %f\n\n",(double) (ed-st)/CLOCKS_PER_SEC);
	//}
	return 0;
}





