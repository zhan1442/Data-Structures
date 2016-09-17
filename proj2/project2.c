#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void counting_sort(int a[], int a_len, int maxVal, int min);
int prep( int * array, int size,  int * largest, int *smallest);
void radix_sort( int a[], int a_len, int largest, int min);
//void quicksort(int *,int,int,int,int);
void quick_sort (int *a, int n);
/*void sort(int * list, int n){
  int min=INT_MAX; int max=INT_MIN;
  int i;
  for (i=0;i<n;i++){
  if (max<list[i]) max=list[i];
  if (min>list[i]) min=list[i];
  }
  quicksort(list,0,n-1,min,max);
  }*/

/*void quicksort(int * list, int d, int u,int min, int max){
  int i=d;
  int j=u;
  int mid=((long int)min+max)/2;
  do {
  while ((list[i]<mid)&&(i<=u)) i++;
  while ((list[j]>mid)&&(j>=d)) j--;
  if (i<j){int temp=list[i]; list[i]=list[j];list[j]=temp;
  i++;j--;}
  } while (i<=j);
  if (i<u)quicksort(list,j+1,u,mid,max);
  if (j>d)quicksort(list,d,j,min,mid);
  }*/

void quick_sort (int *a, int n) {
	int i, j, p, t;
	if (n < 2)
		return;
	p = a[n / 2];
	for (i = 0, j = n - 1;; i++, j--) {
		while (a[i] < p)
			i++;
		while (p < a[j])
			j--;
		if (i >= j)
			break;
		t = a[i];
		a[i] = a[j];
		a[j] = t;
	}
	quick_sort(a, i);
	quick_sort(a + i, n - i);
}


int prep(int * array, int size, int * largest, int *smallest)
{
	int i;
	*largest = array[0];
	*smallest = array[0];

	for(i = 0; i < size; i++){
		if(array[i] > *largest) *largest = array[i];
		else if(array[i] < *smallest) *smallest = array[i];
	}
	//printf("range = %d size = %d m/n = %d",*largest-*smallest,size,(*largest-*smallest)/size);
	return (*largest-*smallest)/size;
}

void radix_sort( int a[], int a_len, int largest, int min)
{
	int i;
	int *b = malloc(a_len*sizeof(int));
	int expn = 1;
	int range = largest-min;

	for (i = 0; range/expn>0; ++i)
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

	for (i = 0; i < bucket_len; i++)
	{
		for (j = 0; j < bucket[i]; j++)
		{
			a[outPos++] = i + min;
		}
	}
	free(bucket);
}

void sort(int list[], int n)
{
	int largest = 0;
	int smallest = 0;
	int mn = prep(list, n, &largest, &smallest);

	if(mn >= 60){
		quick_sort(list,n);//,smallest,largest);
		//printf("\nqSort\n");
		//radix_sort(&list[0], n, largest,smallest);
		//printf("\nRadix Sort\n");
	}
	else {
		counting_sort(&list[0], n, largest, smallest);
		//printf("\ncounting Sort\n");
	}


}

